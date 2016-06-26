#ifndef CDASH_REPORTER_INTERNAL_H
#define CDASH_REPORTER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int CDashPrinter(FILE *, const char *format, ...);
void set_cdash_reporter_printer(CDashPrinter *printer);

#ifdef __cplusplus
}
#endif

#endif
