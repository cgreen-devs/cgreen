#ifndef REPORTER_HEADER
#define REPORTER_HEADER

#include <stdarg.h>

typedef struct TestContext_ TestContext;

typedef struct TestReporter_ TestReporter;
struct TestReporter_ {
    void (*destroy)(TestReporter *);
	void (*start)(TestReporter *, const char *);
	void (*finish)(TestReporter *, const char *);
    void (*show_pass)(TestReporter *, const char *, int, const char *, va_list);
    void (*show_fail)(TestReporter *, const char *, int, const char *, va_list);
    void (*show_incomplete)(TestReporter *, const char *);
    void (*assert_true)(TestReporter *, const char *, int, int, const char *, ...);
	int passes;
	int failures;
	int exceptions;
	void *breadcrumb;
	int ipc;
	void *memo;
};

TestReporter *get_test_reporter();
TestReporter *create_reporter();
void destroy_reporter(TestReporter *reporter);
void reporter_start(TestReporter *reporter, const char *name);
void reporter_finish(TestReporter *reporter, const char *name);
void add_reporter_result(TestReporter *reporter, int result);
void send_reporter_completion_notification(TestReporter *reporter);

#endif
