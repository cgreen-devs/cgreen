#ifndef _XML_REPORTER_HEADER_
#define _XML_REPORTER_HEADER_

#include "cgreen/reporter.h"

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

TestReporter *create_xml_reporter(const char *prefix);

#ifdef __cplusplus
    }
}
#endif

#endif
