#ifndef CUTE_REPORTER_INTERNAL_H
#define CUTE_REPORTER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int CutePrinter(const char *format, ...);
extern void set_cute_reporter_printer(CutePrinter *printer);

#ifdef __cplusplus
}
#endif

#endif
