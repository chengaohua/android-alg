

#ifndef _AIMALL_SDK_INFRA_DEFINE_H_
#define _AIMALL_SDK_INFRA_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

//set visibility of APIs
#if defined _SKIP_DLL_DEFINITIONS_
#define DLL_PUBLIC
#define DLL_LOCAL
#else
#if defined _WIN32 || defined __CYGWIN__
#ifdef tvsTracker_EXPORTS
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif
#endif
#endif

/**
 * 图片数据格式枚举
 */
typedef enum {
    IMO_IMAGE_GRAY8 = 0,
    IMO_IMAGE_BGR565,
    IMO_IMAGE_BGR888,
    IMO_IMAGE_BGRA8888,
    IMO_IMAGE_RGB565,
    IMO_IMAGE_RGB888,
    IMO_IMAGE_RGBA8888,
    IMO_IMAGE_I420,
    IMO_IMAGE_YV12,
    IMO_IMAGE_NV12,
    IMO_IMAGE_NV21,
    IMO_IMAGE_BINARY
}imo_image_format;

/**
 * 人头在图片中的朝向枚举
 */
typedef enum  {
    IMO_IMAGE_UP = 0,     // 人头朝上时，图片逆时针旋转0度后人头朝上
    IMO_IMAGE_DOWN = 180, // 人头朝下时，图片逆时针旋转180度后人头朝上
    IMO_IMAGE_LEFT = 270, // 人头朝左时，图片逆时针旋转270度后人头朝上
    IMO_IMAGE_RIGHT = 90  // 人头朝右时，图片逆时针旋转90度后人头朝上
}imo_image_orientation;

/**
 * imo点数据结构
 */
typedef struct imo_point {
    float x;
    float y;
} imo_point;

/**
 * imo3d点数据结构
 */
typedef struct {
    float x;
    float y;
    float z;
} imo_point_3d;

/**
 * imo框数据结构
 */
typedef struct imo_rect {
    float x;
    float y;
    float width;
    float height;
} imo_rect;

/**
 * imo图片数据结构
 */
typedef struct imo_image {
    /**
     * 图片数据
     */
    const char *data;
    /**
     * 图片的宽
     */
    int width;
    /**
     * 图片的高
     */
    int height;
    /**
     * 图片数据的颜色空间
     */
    imo_image_format format;
} imo_image;

//imo sdk 接口返回错误定义
enum imo_err_code {
  IMO_API_RET_SUCCESS             = 0,    //成功
  IMO_API_RET_INTERNAL_ERR        = -1,   //内部错误
  IMO_API_RET_SDK_NOT_INIT        = -2,   //SDK没有全局初始化
  IMO_API_RET_ALG_NOT_INIT        = -3,   //算法没有初始化

  IMO_API_RET_MODEL_PATH_ERR      = -10,  //模型路径错误

  IMO_API_RET_INVALID_KEY         = -20,  //不合法的授权key
  IMO_API_RET_PERMISSION_DENIED   = -21,  //当前key不具备算法的使用权限

  IMO_API_RET_PARAM_ERR           = -30,  //参数错误
};

#ifdef __cplusplus
}
#endif

#endif //_AIMALL_SDK_INFRA_DEFINE_H_
