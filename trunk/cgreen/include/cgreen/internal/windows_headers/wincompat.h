#ifndef __WINCOMPAT_H__
#define __WINCOMPAT_H__
#ifdef _MSC_VER
#include "stdarg.h"
#include "windows.h"
#include "direct.h"

#define gmtime_r(x,y) gmtime_s(y,x)

typedef int pid_t;

#define snprintf sprintf_s

#define sleep(x) Sleep(x*1000)
#define mkdir(x,y) _mkdir(x)
#define sched_yield() SleepEx (0,0)

#define CGREEN_READ_HANDLE "CGREEN_READ_HANDLE"
#define CGREEN_WRITE_HANDLE "CGREEN_WRITE_HANDLE"
#define CGREEN_TEST_TO_RUN "CGREEN_TEST_TO_RUN"

#ifdef __cplusplus
#define PRIdPTR     "Id"
#endif

#endif //_MSC_VER
#endif //__WINCOMPAT_H__

