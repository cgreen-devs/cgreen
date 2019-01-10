#ifndef TEXT_REPORTER_INTERNAL_H
#define TEXT_REPORTER_INTERNAL_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int TextPrinter(const char *format, ...);
typedef int TextVPrinter(const char *format, va_list arguments);

extern void set_text_reporter_printer(TestReporter *reporter, TextPrinter *printer);
extern void set_text_reporter_vprinter(TestReporter *reporter, TextVPrinter *vprinter);

#ifdef __cplusplus
}
#endif

#endif
