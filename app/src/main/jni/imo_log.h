
#ifndef AIMALL_UNIVERSAL_MODULE_IMO_LOG_H
#define AIMALL_UNIVERSAL_MODULE_IMO_LOG_H

#ifdef __ANDROID__
#undef  LOG_TAG
#define LOG_TAG "AIMALL"
#include <android/log.h>
#else
#include <stdio.h>
#endif

#ifdef SHOWLOG
#ifdef __ANDROID__
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#else  /* !_ANDROID_ */
#include <stdio.h>
#define LOGI(...) \
     do { \
       printf(__VA_ARGS__); printf("\n"); } \
     while (0)
#define LOGD(...) \
     do { \
         printf(__VA_ARGS__); printf("\n");} \
     while (0)
#define LOGW(...) \
     do { \
         printf(__VA_ARGS__); printf("\n");} \
     while (0)
#define LOGE(...) \
     do { \
         printf(__VA_ARGS__); printf("\n");} \
     while (0)
#endif /* !__ANDROID__ */
#else  /* !_DEBUG */
#define LOGI(...)
#define LOGD(...)
#ifdef __ANDROID__
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#else
#define LOGW(...) \
     do { \
         printf(__VA_ARGS__); printf("\n");} \
     while (0)
#define LOGE(...) \
     do { \
         printf(__VA_ARGS__); printf("\n");} \
     while (0)
#endif
#endif


#endif /* __LOG_H__ */
