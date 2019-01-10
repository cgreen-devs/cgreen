#ifndef CUTE_REPORTER_INTERNAL_H
#define CUTE_REPORTER_INTERNAL_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int CutePrinter(const char *format, ...);
typedef int CuteVPrinter(const char *format, va_list arguments);

extern void set_cute_reporter_printer(TestReporter *reporter, CutePrinter *printer);
extern void set_cute_reporter_vprinter(TestReporter *reporter, CuteVPrinter *vprinter);

#ifdef __cplusplus
}
#endif

#endif
