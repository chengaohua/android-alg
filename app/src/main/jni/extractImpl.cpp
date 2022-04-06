#include <iostream>
#include <memory>
#include "imgpreproc.h"
#include "extractImpl.h"



//static cv::Mat preprocess2(const cv::Mat &image, const std::vector<cv::Point2f> &facial5points) {
//
//  std::vector<cv::Point2f> ori_points;
//  ori_points.push_back(facial5points[0]);
//  ori_points.push_back(facial5points[1]);
//  ori_points.push_back(cv::Point2f((facial5points[3].x + facial5points[4].x) / 2,
//                                   (facial5points[3].y + facial5points[4].y) / 2));
//
//  auto final_size = 112;
//  auto trans = imo_cv_transform_warpper(ori_points, dst_points2);
//
//  cv::Mat input;
//
//  cv::warpAffine(image, input, trans, cv::Size(final_size, final_size));
//  return input;
//
//}

namespace face {
Extract::Extract() {
}

Extract::~Extract() {
}

int Extract::init(const char *model_path, int num_threads) {
  //std::string model_file = "/media/gh/Work/sdk/workspace/recognition/v3.0/mnn_v3.0/model/3.0.mnn";
  interpreter_ = std::unique_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path));

  MNN::ScheduleConfig schedule_config;
  schedule_config.type = MNN_FORWARD_CPU;
  schedule_config.numThread = num_threads;
  MNN::BackendConfig backend_config;
  backend_config.precision = MNN::BackendConfig::Precision_Normal;
  schedule_config.backendConfig = &backend_config;

  classifier_sess_ = interpreter_->createSession(schedule_config);
  input_tensor_ = interpreter_->getSessionInput(classifier_sess_, nullptr);

  interpreter_->resizeTensor(input_tensor_, {1, 3, 112, 112});
  interpreter_->resizeSession(classifier_sess_);

  return IMO_API_RET_SUCCESS;
}

int Extract::init(const unsigned char *model_buf, int model_buf_len, int num_threads) {

  return 0;
}

const static int net_width = 112;
const static int net_height = 112;

//static std::vector<cv::Point2f> dst_points2 = {{37.41309091, 43.78181818},
//                                               {75.58676364, 43.78181818},
//                                               {56.44026182, 86.05061818}};

int Extract::exec(const cv::Mat & img, imo_point ori[5], std::vector<float> &feas) {

 // cc::ImageProcess imageProcess(img);

  cc::AFConfig config;
  config.coord_points[0] = 37.41309091;
  config.coord_points[1] = 43.78181818;
  config.coord_points[2] = 75.58676364;
  config.coord_points[3] = 43.78181818;
  config.coord_points[4] = 56.44026182;
  config.coord_points[5] = 86.05061818;

  config.facial_points[0] = ori[0].x;
  config.facial_points[1] = ori[0].y;
  config.facial_points[2] = ori[1].x;
  config.facial_points[3] = ori[1].y;
  config.facial_points[4] = (ori[3].x + ori[4].x) / 2;
  config.facial_points[5] = (ori[3].y + ori[4].y) / 2;


  config.use_points_num = 3;
  config.net_h = net_height;
  config.net_w = net_width;

  cv::Mat alignMat = alignFace2(img,config);

  //cv::Mat input_data = preprocess2(img, ori);

  const float meanVals[3] = {127.5f, 127.5f, 127.5f};
  const float normVals[3] = {3.2f / 255.0f, 3.2f / 255.0f, 3.2f / 255.0f};

  std::shared_ptr<MNN::CV::ImageProcess> pretreat(
      MNN::CV::ImageProcess::create(MNN::CV::BGR, MNN::CV::BGR, meanVals, 3, normVals, 3)
  );

  pretreat->convert((uint8_t *) alignMat.data, net_width, net_height, 3 * net_width, input_tensor_);

  interpreter_->runSession(classifier_sess_);

  MNN::Tensor *output_score = interpreter_->getSessionOutput(classifier_sess_, nullptr);

  // std::cout << output_score->shape()[1] << std::endl;
  // copy to host
  MNN::Tensor score_host(output_score, output_score->getDimensionType());
  output_score->copyToHostTensor(&score_host);

  auto score_ptr = score_host.host<float>();

//  for (int j = 0; j < 256; j++) {
//
//    float score = score_ptr[j];
//    std::cout << score << "\t";
//  }
  std::copy(score_ptr, score_ptr + 256, std::back_inserter(feas));

  return IMO_API_RET_SUCCESS;
}
}