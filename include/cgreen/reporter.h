#ifndef REPORTER_HEADER
#define REPORTER_HEADER

#include <stdarg.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#ifdef _MSC_VER
#define NULL_VA_LIST ""
#elif (defined(__CYGWIN__)) \
 || (defined(__linux__))
#define NULL_VA_LIST NULL
#else
#define NULL_VA_LIST {}
#endif

typedef struct TestContext_ TestContext;

typedef struct TestReporter_ TestReporter;
struct TestReporter_ {
    void (*destroy)(TestReporter *reporter);
    void (*start_suite)(TestReporter *reporter, const char *name, const int count);
    void (*start_test)(TestReporter *reporter, const char *name);
    void (*show_pass)(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
    void (*show_fail)(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
    void (*show_incomplete)(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
    void (*assert_true)(TestReporter *reporter, const char *file, int line, int result, const char * message, ...);
    void (*finish_test)(TestReporter *reporter, const char *file, int line);
    void (*finish_suite)(TestReporter *reporter, const char *file, int line);
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
void reporter_finish(TestReporter *reporter, const char *filename, int line);
void add_reporter_result(TestReporter *reporter, int result);
void send_reporter_completion_notification(TestReporter *reporter);

#ifdef __cplusplus
    }
}
#endif

#endif
