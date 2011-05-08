#ifndef CUTE_REPORTER_HEADER
#define CUTE_REPORTER_HEADER

#include <cgreen/reporter.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

typedef int Printer(const char *format, ...);

TestReporter *create_cute_reporter(void);
void set_cute_printer(TestReporter *reporter, Printer *printer);

#ifdef __cplusplus
    }
}
#endif

#endif
