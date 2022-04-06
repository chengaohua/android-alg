
#include <iostream>
#include "general.h"

#include "imgpreproc.h"
#include "opencv2/opencv.hpp"

cv::Mat imo_cv_transform_warpper(const std::vector<cv::Point2f> &facialPoints,
                                 const std::vector<cv::Point2f> &coordPoints);

namespace cc {
ImageProcess::ImageProcess(imo_image *img) : buf_(nullptr), img_ptr_(nullptr), img_(img) {

}

void ImageProcess::resizeAndFillBlackBorder(RFBConfig &config) {

  cv::Mat bgrMat;
  cc::ImoImage2BgrMat(img_, &bgrMat);

  if (bgrMat.empty()) {
    return;
  }

  cv::resize(bgrMat, bgrMat, cv::Size(config.rs_w, config.rs_h), 0, 0, config.interpolation);

  cv::Mat *stdPatch = new cv::Mat(config.net_h, config.net_w, CV_8UC3, cv::Scalar(0, 0, 0));

  cv::Mat &refMat = *stdPatch;

  int half_offset_x = (config.net_w - config.rs_w) / 2;
  int half_offset_y = (config.net_h - config.rs_h) / 2;

  bgrMat.copyTo(refMat(cv::Rect(half_offset_x, half_offset_y, config.rs_w, config.rs_h)));

  switch (config.dst_format) {
    case CC_RGB : {
      cv::cvtColor(refMat, refMat, cv::COLOR_BGR2RGB);
      break;
    }

    case CC_RGBA: {
      cv::cvtColor(refMat, refMat, cv::COLOR_BGR2RGBA);
      break;
    }

    case CC_BGRA: {
      cv::cvtColor(refMat, refMat, cv::COLOR_BGR2BGRA);

      break;
    }

    case CC_GRAY: {
      cv::cvtColor(refMat, refMat, cv::COLOR_BGR2GRAY);
    }

  }

  buf_ = (char *) stdPatch->data;

  img_ptr_ = stdPatch;

}

void ImageProcess::resize(RConfig &config) {

  cv::Mat bgrMat;
  cc::ImoImage2BgrMat(img_, &bgrMat);

  if (bgrMat.empty()) {
    return;
  }

  cv::Mat *rsMat = new cv::Mat;

  cv::resize(bgrMat, *rsMat, cv::Size(config.rs_w, config.rs_h), 0, 0, config.interpolation);

  switch (config.dst_format) {
    case CC_RGB : {
      cv::cvtColor(*rsMat, *rsMat, cv::COLOR_BGR2RGB);
      break;
    }

    case CC_RGBA: {
      cv::cvtColor(*rsMat, *rsMat, cv::COLOR_BGR2RGBA);
      break;
    }

    case CC_BGRA: {
      cv::cvtColor(*rsMat, *rsMat, cv::COLOR_BGR2BGRA);
      break;
    }

    case CC_GRAY: {
      cv::cvtColor(*rsMat, *rsMat, cv::COLOR_BGR2GRAY);
    }

  }

  buf_ = (char *) rsMat->data;

  img_ptr_ = rsMat;

}

void ImageProcess::alignFace(AFConfig &config) {

  cv::Mat bgrMat;
  cc::ImoImage2BgrMat(img_, &bgrMat);

  if (bgrMat.empty()) {
    return;
  }

  std::vector<cv::Point2f> oriPoints;
  std::vector<cv::Point2f> dstPoints;

  for (int i = 0; i < config.use_points_num; i++) {
    oriPoints.emplace_back(cv::Point2f(config.facial_points[i * 2], config.facial_points[i * 2 + 1]));
    dstPoints.emplace_back(cv::Point2f(config.coord_points[i * 2], config.coord_points[i * 2 + 1]));
  }

  auto trans = imo_cv_transform_warpper(oriPoints, dstPoints);

  cv::Mat *alignMat = new cv::Mat;
  cv::warpAffine(bgrMat, *alignMat, trans, cv::Size(config.net_w, config.net_h));

//  cv::imshow("crop", *alignMat);
//  cv::waitKey(1);
  buf_ = (char *) alignMat->data;

  img_ptr_ = alignMat;

}

ImageProcess::~ImageProcess() {
  if (img_ptr_) {
    delete (cv::Mat * )(img_ptr_);
  }

}


cv::Mat alignFace2(const cv::Mat & img,AFConfig &config) {

  std::vector<cv::Point2f> oriPoints;
  std::vector<cv::Point2f> dstPoints;

  for (int i = 0; i < config.use_points_num; i++) {
    oriPoints.emplace_back(cv::Point2f(config.facial_points[i * 2], config.facial_points[i * 2 + 1]));
    dstPoints.emplace_back(cv::Point2f(config.coord_points[i * 2], config.coord_points[i * 2 + 1]));
  }

  auto trans = imo_cv_transform_warpper(oriPoints, dstPoints);

  cv::Mat alignMat ;
  cv::warpAffine(img, alignMat, trans, cv::Size(config.net_w, config.net_h));

  return alignMat;
}

}




void ULsee_rigid_transform(const cv::Mat &A, const cv::Mat &B, cv::Mat &R,
                           cv::Mat &T, float &scale) {
  assert(A.cols == B.cols);
  assert(A.rows == B.rows);
  cv::Mat meanA, centroidA, meanB, centroidB;
  cv::reduce(A, meanA, 0, CV_REDUCE_AVG);
  cv::repeat(meanA, A.rows, 1, centroidA);
  cv::Mat AA = A - centroidA;
  cv::reduce(B, meanB, 0, CV_REDUCE_AVG);
  cv::repeat(meanB, B.rows, 1, centroidB);
  cv::Mat BB = B - centroidB;
  cv::Mat H = AA.t() * BB / A.rows;
  cv::SVD decomp = cv::SVD(H);
  cv::Mat S, U, V_t;
  S = decomp.w;
  U = decomp.u;
  V_t = decomp.vt;
  if (cv::determinant(U) * cv::determinant(V_t) < 0) {
    S.row(S.rows - 1) *= -1.;
    U.col(U.cols - 1) *= -1.;
  }
  R = U * V_t;
  float varP = 0;
  //std::cout << A << std::endl;
  for (int i = 0; i < A.size().width; i++) {
    cv::Scalar mean, var;
    cv::meanStdDev(A.col(i), mean, var);
    varP += cv::sum(var * var)[0];
  }
  scale = 1.0 / varP * cv::sum(S)[0];
  T = meanB - meanA * (scale * R);
}

cv::Mat imo_cv_transform_warpper(const std::vector<cv::Point2f> &facialPoints,
                                  const std::vector<cv::Point2f> &coordPoints) {
  int len = facialPoints.size();
  float aData[len][2], bData[len][2];
  for (int i = 0; i < len; i++) {
    aData[i][0] = facialPoints[i].x;
    aData[i][1] = facialPoints[i].y;
    bData[i][0] = coordPoints[i].x;
    bData[i][1] = coordPoints[i].y;
  }
  // imo::Mat A(facialPoints), B(coordPoints);
  cv::Mat A = cv::Mat(len, 2, CV_32FC1, &aData),
      B = cv::Mat(len, 2, CV_32FC1, &bData);

  CV_Assert(A.type() == CV_32F || A.type() == CV_64F);
  cv::Mat R, T;
  float scale;
  ULsee_rigid_transform(A, B, R, T, scale);
  R = R * scale;

  cv::Mat tt;
  cv::repeat(T, len, 1, tt);
  cv::Mat diff = (A * R + tt) - B;

  cv::hconcat(R.t(), T.t(), R);

  return R;
}


