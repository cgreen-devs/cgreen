#ifndef CDASH_REPORTER_HEADER
#define CDASH_REPORTER_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/reporter.h>
#include <stdio.h>

typedef struct CDashInfo_ CDashInfo;
struct CDashInfo_ {
	char *name;
	char *build;
	char *type;
};

TestReporter *create_cdash_reporter(CDashInfo *cdash);

#ifdef __cplusplus
    }
#endif

#endif
