/** @file slog_api.h
  * @brief log库
  * @author Captain China
  * @date 2019/02/07
  */
#ifndef _SLOG_API_D435045C_1E0B_416D_8C6B_EABFEAADA744_H_
#define _SLOG_API_D435045C_1E0B_416D_8C6B_EABFEAADA744_H_

#include <stdio.h>

#define WATCH_DELAY_TIME     10 * 1000

namespace him {

	class LogObject;

	class Log
	{
	public:

#ifdef DISABLESLOG
		Log(const char* module_name, int delay = WATCH_DELAY_TIME) {}
		virtual ~Log() {}
		void Trace(const char* format, ...) {}
		void Debug(const char* format, ...) {}
		void Info(const char* format, ...) {}
		void Warn(const char* format, ...) {}
		void Error(const char* format, ...) {}
		void Fatal(const char* format, ...) {}
#else 
		Log(const char* module_name, int delay = WATCH_DELAY_TIME);
		virtual ~Log();
		void Trace(const char* format, ...);
		void Debug(const char* format, ...);
		void Info(const char* format, ...);
		void Warn(const char* format, ...);
		void Error(const char* format, ...);
		void Fatal(const char* format, ...);

#endif
	};

}

// 对以上类的宏封装，方便使用

#define LOG_MODULE_IM         "IM"

extern him::Log g_himlog;

/*
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#if defined(_WIN32) || defined(_WIN64)
#define logd(fmt, ...)  g_himlog.Debug("<%s>\t<%d>\t<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define logi(fmt, ...)  g_himlog.Info("<%s>\t<%d>\t<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define logw(fmt, ...)  g_himlog.Warn("<%s>\t<%d>\t<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define loge(fmt, ...)  g_himlog.Error("<%s>\t<%d>\t<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define logfa(fmt, ...)  g_himlog.Fatal("<%s>\t<%d>\t<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define log(fmt, args...)  g_himlog.Info("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#endif
*/

#ifdef DISABLESLOG
#define logd(fmt, ...)
#define logi(fmt, ...)
#define logw(fmt, ...)
#define loge(fmt, ...)
#define logfa(fmt, ...)
#else
#define logd(fmt, ...)  printf("[DEBUG]\t<%s>\t<%d>" fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define logi(fmt, ...)  printf("[INFO]\t<%s>\t<%d>" fmt "\n", __FUNCTION__,__LINE__, ##__VA_ARGS__)
#define logw(fmt, ...) printf("[WARN]\t<%s>\t<%d>" fmt "\n", __FUNCTION__,__LINE__, ##__VA_ARGS__)
#define loge(fmt, ...)  printf("[ERROR]\t<%s>\t<%d>" fmt "\n", __FUNCTION__,__LINE__, ##__VA_ARGS__)
#define logfa(fmt, ...)  printf("[FATAL]\t<%s>\t<%d>" fmt "\n", __FUNCTION__,__LINE__, ##__VA_ARGS__)
#endif

#endif//_SLOG_API_D435045C_1E0B_416D_8C6B_EABFEAADA744_H_
