#ifndef CUTE_REPORTER_HEADER
#define CUTE_REPORTER_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/reporter.h>

typedef int Printer(const char *format, ...);

TestReporter *create_cute_reporter(void);
void set_cute_printer(TestReporter *reporter, Printer *printer);

#ifdef __cplusplus
    }
#endif

#endif
