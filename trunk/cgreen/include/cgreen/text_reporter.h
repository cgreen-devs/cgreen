#ifndef TEXT_REPORTER_HEADER
#define TEXT_REPORTER_HEADER

#include <cgreen/reporter.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

TestReporter *create_text_reporter(void);

#ifdef __cplusplus
    }
}
#endif

#endif
