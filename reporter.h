#ifndef _REPORTER_HEADER_
#define _REPORTER_HEADER_

#include <stdarg.h>

typedef struct _TestReporter TestReporter;
struct _TestReporter {
    void (*destroy)(TestReporter *);
	void (*start)(TestReporter *, const char *name);
	void (*finish)(TestReporter *, const char *name);
    void (*show_pass)(TestReporter *, const char *, int, char *, va_list);
    void (*show_fail)(TestReporter *, const char *, int, char *, va_list);
    void (*show_incomplete)(TestReporter *, const char *);
    void (*assert_true)(TestReporter *, const char *, int, int, char *, ...);
	int passes;
	int failures;
	int exceptions;
	void *breadcrumb;
	int ipc;
	void *memo;
};

TestReporter *create_reporter();
void destroy_reporter(TestReporter *reporter);
void reporter_start(TestReporter *reporter, const char *name);
void reporter_finish(TestReporter *reporter, const char *name);
void add_reporter_result(TestReporter *reporter, int result);
void send_reporter_completion_notification(TestReporter *reporter);

#endif
