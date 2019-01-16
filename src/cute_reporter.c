#include <cgreen/cute_reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cute_reporter_internal.h"

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

typedef struct {
    CutePrinter *printer;
    CuteVPrinter *vprinter;
    int error_count;    // For status within the test case process
    int previous_error; // For status outside the test case process
} CuteMemo;

static void cute_start_suite(TestReporter *reporter,
        const char *name, const int number_of_tests);
static void cute_start_test(TestReporter *reporter,
        const char *name);
static void show_fail(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments);
static void show_pass(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments);
static void cute_failed_to_complete(TestReporter *reporter,
        const char *file, int line, const char *message, va_list arguments);
static void cute_finish_test(TestReporter *reporter,
        const char *filename, int line, const char *message);
static void cute_finish_suite(TestReporter *reporter,
        const char *filename, int line);


void set_cute_reporter_printer(TestReporter *reporter, CutePrinter *new_printer) {
    CuteMemo *memo = (CuteMemo *)reporter->memo;
    memo->printer = new_printer;
}

void set_cute_reporter_vprinter(TestReporter *reporter, CuteVPrinter *new_vprinter) {
    CuteMemo *memo = (CuteMemo *)reporter->memo;
    memo->vprinter = new_vprinter;
}

TestReporter *create_cute_reporter(void) {
    CuteMemo *memo;
    TestReporter *reporter;

    reporter = create_reporter();
    if (reporter == NULL) {
        return NULL;
    }

    memo = (CuteMemo *) malloc(sizeof(CuteMemo) + 100);
    if (memo == NULL) {
        destroy_reporter(reporter);
        return NULL;
    }
    reporter->memo = memo;
    
    set_cute_reporter_printer(reporter, printf);
    set_cute_reporter_vprinter(reporter, vprintf);

    reporter->start_suite = &cute_start_suite;
    reporter->start_test = &cute_start_test;
    reporter->show_fail = &show_fail;
    reporter->show_pass = &show_pass;
    reporter->show_incomplete = &cute_failed_to_complete;
    reporter->finish_test = &cute_finish_test;
    reporter->finish_suite = &cute_finish_suite;
    reporter->memo = memo;

    return reporter;
}

static void cute_start_suite(TestReporter *reporter,
                             const char *name, const int number_of_tests) {
    reporter->passes = 0;
    reporter->failures = 0;
    reporter->skips = 0;
    reporter->exceptions = 0;

    CuteMemo *memo = (CuteMemo *)reporter->memo;
    reporter_start_test(reporter, name);
    memo->printer("#beginning %s %d\n", name, number_of_tests);
}

static void cute_start_test(TestReporter *reporter,
        const char *name) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    memo->error_count = reporter->failures + reporter->exceptions;
    memo->previous_error = 0;
    reporter_start_test(reporter, name);
    memo->printer("#starting %s\n", name);
}

static void cute_finish_test(TestReporter *reporter, const char *filename, int line, const char *message) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);

    reporter_finish_test(reporter, filename, line, message);
    if (memo->error_count == reporter->failures + reporter->exceptions) {
        memo->printer("#success %s, %d ms OK\n", name);
    }
}

static void cute_finish_suite(TestReporter *reporter, const char *filename, int line) {
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
    CuteMemo *memo = (CuteMemo *)reporter->memo;
    
    reporter_finish_test(reporter, filename, line, NULL);

    reporter->total_passes += reporter->passes;
    reporter->total_failures += reporter->failures;
    reporter->total_skips += reporter->skips;
    reporter->total_exceptions += reporter->exceptions;

    memo->printer("#ending %s", name);
    if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 0) {
        memo->printer(": %d pass%s, %d failure%s, %d exception%s, %d ms.\n",
                      reporter->passes, reporter->passes == 1 ? "" : "es",
                      reporter->failures, reporter->failures == 1 ? "" : "s",
                      reporter->exceptions, reporter->exceptions == 1 ? "" : "s",
                      reporter->total_duration);
    } else
        memo->printer("\n");
}

static void show_fail(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    if (!memo->previous_error) {
        memo->printer("#failure %s",
                      get_current_from_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb));
        memo->printer(" %s:%d ", file, line);
        if (message == NULL) {
            memo->printer("<FATAL: NULL for failure message>");
        } else {
            memo->vprinter(message, arguments);
        }
        memo->printer("\n");
        memo->previous_error = 1;
    }
}

static void show_pass(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments) {
    (void) reporter;
    (void) file;
    (void) line;
    (void) message;
    (void) arguments;
}

static void cute_failed_to_complete(TestReporter *reporter,
        const char *file, int line, const char *message, va_list arguments) {
    CuteMemo *memo = (CuteMemo *)reporter->memo;

    /* TODO: add additional message to output */
    (void)file;
    (void)line;
    (void)message;
    (void)arguments;

    memo->printer("#error %s failed to complete\n",
                  get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb));
}
/* vim: set ts=4 sw=4 et cindent: */
