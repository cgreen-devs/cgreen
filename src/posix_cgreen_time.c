#include "cgreen/internal/cgreen_time.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

/* TODO: this should really be handle by CMake config... */
#if defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
#  include <sys/time.h>
#  define HAVE_GETTIMEOFDAY 1
#else
#  error "Your POSIX platform does not support gettimeofday(). Please report a bug to http://github.com/cgreen-devs/cgreen/issues"
#endif

#if defined(HAVE_GETTIMEOFDAY)
uint32_t cgreen_time_get_current_milliseconds() {
#ifdef __CYGWIN__
    /* TODO: This is actually the POSIX recommended way to do this */
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        fprintf(stderr, "cgreen error: could not get time\n");
        return 0;
    }

    return ts.tv_sec * 1000u + ts.tv_nsec / 1000000u;
#else    
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0) {
        fprintf(stderr, "cgreen error: could not get time of day\n");
        return 0;
    }
    return tv.tv_sec * 1000u + tv.tv_usec / 1000u;
#endif
}
#endif


/* vim: set ts=4 sw=4 et cindent: */
