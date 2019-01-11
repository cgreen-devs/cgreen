#ifndef CDASH_REPORTER_INTERNAL_H
#define CDASH_REPORTER_INTERNAL_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int CDashPrinter(FILE *, const char *format, ...);
typedef int CDashVPrinter(FILE *, const char *format, va_list arguments);
void set_cdash_reporter_printer(TestReporter *reporter, CDashPrinter *printer);
void set_cdash_reporter_vprinter(TestReporter *reporter, CDashVPrinter *vprinter);

#ifdef __cplusplus
}
#endif

#endif
