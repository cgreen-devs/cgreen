#ifndef CDASH_REPORTER_HEADER
#define CDASH_REPORTER_HEADER

#include <cgreen/reporter.h>
#include <stdio.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

typedef struct CDashInfo_ CDashInfo;
struct CDashInfo_ {
	char *name;
	char *build;
	char *type;
	char *hostname;
	char *os_name;
	char *os_platform;
	char *os_release;
	char *os_version;
};

typedef int CDashPrinter(FILE *, const char *format, ...);

extern void set_cdash_printer(TestReporter *reporter, CDashPrinter *printer);
extern TestReporter *create_cdash_reporter(CDashInfo *cdash);

#ifdef __cplusplus
    }
}
#endif

#endif
