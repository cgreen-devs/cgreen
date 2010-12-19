#ifndef REPORTER_HEADER
#define REPORTER_HEADER

#ifdef __cplusplus
  extern "C" {
#endif


#include <stdarg.h>

typedef struct TestContext_ TestContext;

typedef struct TestReporter_ TestReporter;
struct TestReporter_ {
    void (*destroy)(TestReporter *);
	void (*start_suite)(TestReporter *, const char *, const int);
	void (*start_test)(TestReporter *, const char *);
    void (*show_pass)(TestReporter *, const char *, int, const char *, va_list);
    void (*show_fail)(TestReporter *, const char *, int, const char *, va_list);
    void (*show_incomplete)(TestReporter *, const char *);
    void (*assert_true)(TestReporter *, const char *, int, int, const char *, ...);
	void (*finish_test)(TestReporter *, const char *);
	void (*finish_suite)(TestReporter *, const char *);
	int passes;
	int failures;
	int exceptions;
	void *breadcrumb;
	int ipc;
	void *memo;
};

typedef void TestReportMemo;

TestReporter *get_test_reporter(void);
TestReporter *create_reporter(void);
void setup_reporting(TestReporter *reporter);
void destroy_reporter(TestReporter *reporter);
void destroy_memo(TestReportMemo *memo);
void reporter_start(TestReporter *reporter, const char *name);
void reporter_start_suite(TestReporter *reporter, const char *name,
    const int count);
void reporter_finish(TestReporter *reporter, const char *name);
void add_reporter_result(TestReporter *reporter, int result);
void send_reporter_completion_notification(TestReporter *reporter);

#ifdef __cplusplus
    }
#endif


#endif
