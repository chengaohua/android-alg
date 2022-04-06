
#ifndef IMO_DEEPLEARNING_PRE_PROCESS_HPP
#define IMO_DEEPLEARNING_PRE_PROCESS_HPP

#include "imo_define.h"
#include "opencv2/opencv.hpp"

namespace cc {

//resize 方法
enum {
  CC_INTER_NN = 0,
  CC_INTER_LINEAR = 1,
  CC_INTER_CUBIC = 2,
  CC_INTER_AREA = 3
};

//目标格式，暂时只会用到这几种
enum {
  CC_RGBA = 0,
  CC_RGB,
  CC_BGR,
  CC_BGRA,
  CC_GRAY,
};

//缩放然后填黑边配置,针对检测器配置
typedef struct {
  int rs_h;                   // resize后图片高度
  int rs_w;                   // resize后图片高度
  int net_h;                       //神经网络输入高度
  int net_w;                     // 神经网络输入宽度
  int interpolation;      //resize方法
  int dst_format;         //目标类型

} RFBConfig;

//缩放
typedef struct {
  int rs_h;   // resize后图片高度
  int rs_w;// resize后图片高度
  int interpolation; //resize方法
  int dst_format;//目标类型

} RConfig;

//人脸对齐
typedef struct {

  float coord_points[10];        //x,y,x,y　　矫正关键点
  float facial_points[10];       //x, y, x, y　人脸关键点
  int use_points_num;           // 矫正人脸数  ，存在５个点场景和３个点场景

  int net_h;//神经网络输入高度
  int net_w; // 神经网络输入宽度
} AFConfig;

class ImageProcess {

 public:

  ImageProcess(imo_image *img);

  ~ImageProcess();

  ImageProcess(const ImageProcess &) = delete;

  ImageProcess &operator=(const ImageProcess &) = delete;

  /**
   *  缩放并补黑边
   */
  void resizeAndFillBlackBorder(RFBConfig &config);

  /**
  * 人脸对齐
  */
  void alignFace(AFConfig &config);

 // cv::Mat alignFace2(AFConfig &config);

  /**
  * 缩放
  */
  void resize(RConfig &config);

 public:
  char *buf_;

 private:

  void *img_ptr_;

  imo_image *img_;

};


  cv::Mat alignFace2(const cv::Mat & img,AFConfig &config);

}

#endif //IMO_DEEPLEARNING_PRE_PROCESS_HPP
