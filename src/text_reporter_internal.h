#ifndef TEXT_REPORTER_INTERNAL_H
#define TEXT_REPORTER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int TextPrinter(const char *format, ...);
extern void set_text_reporter_printer(TextPrinter *printer);

#ifdef __cplusplus
}
#endif

#endif
