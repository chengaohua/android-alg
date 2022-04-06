#pragma once

#include "opencv2/opencv.hpp"
#include <vector>


#include "MNN/Interpreter.hpp"
#include "MNN/ImageProcess.hpp"
#include "MNN/MNNDefine.h"
#include "MNN/Tensor.hpp"

using namespace std;
using namespace cv;

namespace face {
class Extract final {
 public:
  Extract();
  ~Extract();

  int init(const char *model_path, int num_threads);
  int init(const unsigned char *model_buf, int model_buf_len, int num_threads);

  int exec(const cv::Mat & img, imo_point ori[5], std::vector<float> &feas);
 private:
  std::unique_ptr<MNN::Interpreter> interpreter_;

  MNN::Session *classifier_sess_ = nullptr;
  MNN::Tensor *input_tensor_ = nullptr;
};
}

