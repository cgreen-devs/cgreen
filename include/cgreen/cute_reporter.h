#ifndef CUTE_REPORTER_HEADER
#define CUTE_REPORTER_HEADER

#include <cgreen/reporter.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

typedef int CutePrinter(const char *format, ...);


extern TestReporter *create_cute_reporter(void);
extern void set_cute_printer(TestReporter *reporter, CutePrinter *printer);

#ifdef __cplusplus
    }
}
#endif

#endif
