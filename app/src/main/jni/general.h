//
// Created by zhangchao on 18-9-17.
//

#ifndef _IMO_SDK_CORE_GENERAL_H_
#define _IMO_SDK_CORE_GENERAL_H_

#include "opencv2/opencv.hpp"
#include "imo_define.h"
//#include "general.h"

namespace cc {

// imo_image结构转化为opencv bgr mat
    int  ImoImage2BgrMat(const imo_image *image, cv::Mat *bgrMat);

// imo_image结构转化为opencv rgb mat
    int  ImoImage2RgbMat(const imo_image *image, cv::Mat *bgrMat);

// imo_image结构转化为opencv gray mat
    int  ImoImage2GrayMat(const imo_image *image, cv::Mat *grayMat);

// 逆时针旋转mat
    int  RotateMatCCW(cv::Mat *mat, int degreesCCW);

// 逆时针旋转mat后转置
    int  RotateMatCCWAndTranspose(cv::Mat *mat, int degreesCCW);

// 顺时针旋转点位
    int  RotatePointsCW(float *points, unsigned int length, int width, int height, int degreesCW);

} // name space imo

#endif //_IMO_SDK_CORE_GENERAL_H_
