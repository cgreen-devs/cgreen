#ifndef REPORTER_HEADER
#define REPORTER_HEADER

#include <stdarg.h>
#include <stdint.h>
#include <cgreen/breadcrumb.h>

typedef struct TestReporter_ TestReporter;
struct TestReporter_ {
    void (*destroy)(TestReporter *reporter);
    void (*start_suite)(TestReporter *reporter, const char *name, const int count);
    void (*start_test)(TestReporter *reporter, const char *name);
    void (*show_pass)(TestReporter *reporter, const char *file, int line,
                      const char *message, va_list arguments);
    void (*show_skip)(TestReporter *reporter, const char *file, int line);
    void (*show_fail)(TestReporter *reporter, const char *file, int line,
                      const char *message, va_list arguments);
    void (*show_incomplete)(TestReporter *reporter, const char *file, int line,
                            const char *message, va_list arguments);
    void (*assert_true)(TestReporter *reporter, const char *file, int line,
                        int result, const char * message, ...);
    void (*finish_test)(TestReporter *reporter, const char *file, int line,
                        const char *message, uint32_t duration_in_milliseconds);
    void (*finish_suite)(TestReporter *reporter, const char *file, int line,
                         uint32_t milliseconds);
    int passes;
    int failures;
    int exceptions;
    int skips;
    CgreenBreadcrumb *breadcrumb;
    int ipc;
    void *memo;
    void *options;
};

typedef void TestReportMemo;

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

TestReporter *create_reporter(void);
TestReporter *get_test_reporter(void);
void set_reporter_options(TestReporter *reporter, void *options);
void setup_reporting(TestReporter *reporter);
void destroy_reporter(TestReporter *reporter);
void destroy_memo(TestReportMemo *memo);
void reporter_start_test(TestReporter *reporter, const char *name);
void reporter_start_suite(TestReporter *reporter, const char *name, const int count);
void reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message,
                     uint32_t duration_in_milliseconds);
void reporter_finish_suite(TestReporter *reporter, const char *filename, int line, uint32_t duration_in_milliseconds);
void add_reporter_result(TestReporter *reporter, int result);
void send_reporter_exception_notification(TestReporter *reporter);
void send_reporter_skipped_notification(TestReporter *reporter);
void send_reporter_completion_notification(TestReporter *reporter);

// Exposing so user code can call this manually to update pass/fail counts
// for later analysis when running tests singly (instead of complete test suite)
int read_reporter_results(TestReporter *reporter);

#ifdef __cplusplus
    }
}
#endif

#endif
