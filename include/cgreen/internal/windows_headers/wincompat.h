#ifndef WINCOMPAT_H
#define WINCOMPAT_H
#ifdef __WIN32__
#include "stdarg.h"
#include "windows.h"
#include "direct.h"
#include <shlwapi.h>

#define gmtime_r(x,y) gmtime_s(y,x)

// This was not necessary on Msys/Mingw32/64
//typedef int pid_t;

#if _MSC_VER < 1900
   #define snprintf sprintf_s
#endif

#define sleep(x) Sleep(x*1000)
#define mkdir(x,y) _mkdir(x)
#define sched_yield() SleepEx (0,0)

#define CGREEN_READ_HANDLE "CGREEN_READ_HANDLE"
#define CGREEN_WRITE_HANDLE "CGREEN_WRITE_HANDLE"
#define CGREEN_TEST_TO_RUN "CGREEN_TEST_TO_RUN"


/* Simple replacement for isatty() */
#define isatty(x) 1

/* Simple replacement for setenv() */
#define setenv(variable, value, overwrite) SetEnvironmentVariable(variable, value)


#endif //__WIN32__
#endif //WINCOMPAT_H
