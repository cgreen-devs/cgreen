/**
 * Convert *printf family for display via Android Studio/Eclipse logcat
 *
 * Usage
 *      Simply include in cgreen .c/.cpp files containing *printf statements e.g.
 *      ------------------------------------------------------------
 *      #ifdef __ANDROID__
 *      #include "cgreen/internal/android_headers/androidcompat.h"
 *      #endif // #ifdef __ANDROID__
 *      ------------------------------------------------------------
 *
 * Created 9 Aug 2016, Steve Madsen, London UK
 */
#pragma once

#ifdef __ANDROID__
#include <android/log.h>
#define ANDROID_MODULE_LOG_TAG  "CGreenUnitTests"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, ANDROID_MODULE_LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, ANDROID_MODULE_LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, ANDROID_MODULE_LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, ANDROID_MODULE_LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, ANDROID_MODULE_LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, ANDROID_MODULE_LOG_TAG, __VA_ARGS__)

// Note: "##" ensures preceding comma will be deleted if "..." empty
#define printf(fmt, ...) LOGE(fmt, ##__VA_ARGS__)
#define fprintf(stderr, ...) LOGE(__VA_ARGS__)
#define vprintf(fmt, ...) LOGE(fmt, ##__VA_ARGS__)
#define snprintf(buf, len, fmt, ...) LOGE(fmt, ##__VA_ARGS__)
#define sprintf(buf, fmt, ...) LOGE(fmt, ##__VA_ARGS__)
#endif // #ifdef __ANDROID__
