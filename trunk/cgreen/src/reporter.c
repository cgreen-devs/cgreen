#include <cgreen/reporter.h>
#include <cgreen/messaging.h>
#include <cgreen/breadcrumb.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
namespace cgreen {
#endif

enum { pass = 1, fail, completion, exception };
enum { FINISH_NOTIFICATION_RECEIVED = 0, FINISH_NOTIFICATION_NOT_RECEIVED };

static TestContext context;

static void show_pass(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void assert_true(TestReporter *reporter, const char *file, int line, int result, const char *message, ...);
static int  read_reporter_results(TestReporter *reporter);

TestReporter *get_test_reporter() {
    return context.reporter;
}

void setup_reporting(TestReporter *reporter) {
    reporter->ipc = start_cgreen_messaging(45);
    context.reporter = reporter;
}

void set_reporter_options(TestReporter *reporter, void *options) {
    if (reporter->options != NULL)
        free(reporter->options);
    reporter->options = options;
}

TestReporter *create_reporter() {
    CgreenBreadcrumb *breadcrumb;
    TestReporter *reporter = (TestReporter *) malloc(sizeof(TestReporter));
    if (reporter == NULL) {
        return NULL;
    }

    breadcrumb = create_breadcrumb();
    if (breadcrumb == NULL) {
        free(reporter);
        return NULL;
    }

    reporter->destroy = &destroy_reporter;
    reporter->start_suite = &reporter_start_suite;
    reporter->start_test = &reporter_start;
    reporter->show_pass = &show_pass;
    reporter->show_fail = &show_fail;
    reporter->show_incomplete = &show_incomplete;
    reporter->assert_true = &assert_true;
    reporter->finish_test = &reporter_finish;
    reporter->finish_suite = &reporter_finish_suite;
    reporter->passes = 0;
    reporter->failures = 0;
    reporter->exceptions = 0;
    reporter->breadcrumb = breadcrumb;
    reporter->memo = NULL;
    reporter->options = NULL;
    return reporter;
}

void destroy_reporter(TestReporter *reporter) {
    destroy_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
    destroy_memo((TestReportMemo *)reporter->memo);
    free(reporter);
    //hack to allow destroy_reporter to be called in cute_reporter_tests when
    //tests are running in same process
    if (context.reporter == reporter)
    {
        context.reporter = NULL;
    }
}

void destroy_memo(TestReportMemo *memo) {
    if (NULL != memo) {
        free(memo);
    }
}

void reporter_start(TestReporter *reporter, const char *name) {
    push_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, name);
}

void reporter_start_suite(TestReporter *reporter, const char *name, const int count) {
    (void) count;
    reporter_start(reporter, name);
}

void reporter_finish(TestReporter *reporter, const char *filename, int line, const char *message) {
    int status = read_reporter_results(reporter);

    if (status == FINISH_NOTIFICATION_NOT_RECEIVED) {
        va_list no_arguments;
        memset(&no_arguments, 0, sizeof(va_list));
        reporter->exceptions++;
        (*reporter->show_incomplete)(reporter, filename, line, message, no_arguments);
    }

    pop_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
}

void reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    (void) filename;
    (void) line;
    read_reporter_results(reporter);
    pop_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
}

void add_reporter_result(TestReporter *reporter, int result) {
    send_cgreen_message(reporter->ipc, result ? pass : fail);
}

void send_reporter_exception_notification(TestReporter *reporter) {
    send_cgreen_message(reporter->ipc, exception);
}

void send_reporter_completion_notification(TestReporter *reporter) {
    send_cgreen_message(reporter->ipc, completion);
}

static void show_pass(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    (void)reporter;
    (void)file;
    (void)line;
    (void)message;
    (void)arguments;
}

static void show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    (void)reporter;
    (void)file;
    (void)line;
    (void)message;
    (void)arguments;
}

static void show_incomplete(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    (void)reporter;
    (void)file;
    (void)line;
    (void)message;
    (void)arguments;
}

static const unsigned int MAX_ASSERTIONS_PER_TEST = 8100;

static void assert_true(TestReporter *reporter, const char *file, int line, int result, const char *message, ...) {
    va_list arguments;
    memset(&arguments, 0, sizeof(va_list));

    va_start(arguments, message);
    if (result) {
            (*reporter->show_pass)(reporter, file, line, message, arguments);
    } else {
            (*reporter->show_fail)(reporter, file, line, message, arguments);
    }

    add_reporter_result(reporter, result);
    va_end(arguments);
}

static int read_reporter_results(TestReporter *reporter) {
    int result;
    while ((result = receive_cgreen_message(reporter->ipc)) > 0) {
        if (result == pass) {
            reporter->passes++;
        } else if (result == fail) {
            reporter->failures++;
        } else if (result == exception) {
            reporter->exceptions++;
        } else if (result == completion) {
            /* TODO: this should always be the last message; if it's not, there's a bad race */
            return FINISH_NOTIFICATION_RECEIVED;
        }
    }

    return FINISH_NOTIFICATION_NOT_RECEIVED;
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
