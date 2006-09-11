#ifndef _REPORTER_HEADER_
#define _REPORTER_HEADER_

#include <stdarg.h>

typedef struct _TestReporter TestReporter;
struct _TestReporter {
    void (*destroy)(TestReporter *);
	void (*start)(TestReporter *, char *name);
	void (*finish)(TestReporter *, char *name);
    void (*show_pass)(TestReporter *, const char *, int, char *, va_list);
    void (*show_fail)(TestReporter *, const char *, int, char *, va_list);
    void (*show_incomplete)(TestReporter *, char *);
    void (*assert_true)(TestReporter *, const char *, int, int, char *, ...);
	int passes;
	int failures;
	int exceptions;
	char **breadcrumb;
	int breadcrumb_depth;
	int breadcrumb_space;
	int ipc;
	void *memo;
};

TestReporter *create_reporter();
void destroy_reporter(TestReporter *reporter);
void reporter_start(TestReporter *reporter, char *name);
void reporter_finish(TestReporter *reporter, char *name);
void add_reporter_result(TestReporter *reporter, int result);
void send_reporter_completion_notification(TestReporter *reporter);
char *get_current_reporter_test(TestReporter *reporter);
void walk_reporter_breadcrumb(TestReporter *reporter, void (*walker)(char *, void *), void *memo);

#endif
