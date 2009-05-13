#include <cgreen/cute_reporter.h>
#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	Printer *printer;
	int error_count;	// For status within the test case process
	int previous_error;	// For status outside the test case process
} CuteMemo;

static void cute_reporter_suite_started(TestReporter *reporter, const char *name, const int number_of_tests);
static void cute_reporter_testcase_started(TestReporter *reporter, const char *name);
static void assert_failed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void assert_passed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void testcase_failed_to_complete(TestReporter *reporter, const char *name);
static void cute_reporter_testcase_finished(TestReporter *reporter, const char *name);
static void cute_reporter_suite_finished(TestReporter *reporter, const char *name);


void set_cute_printer(TestReporter *reporter, Printer *printer) {
	CuteMemo *memo = (CuteMemo *)reporter->memo;
	memo->printer = printer;
}

TestReporter *create_cute_reporter(void) {
    CuteMemo *memo;
    TestReporter *reporter;

    reporter = create_reporter();
    if (reporter == NULL) {
        return NULL;
    }

    memo = malloc(sizeof(CuteMemo) + 100);
    if (memo == NULL) {
        destroy_reporter(reporter);
        return NULL;
    }
	memo->printer = printf;

	reporter->start_suite = &cute_reporter_suite_started;
	reporter->start_test = &cute_reporter_testcase_started;
	reporter->show_fail = &assert_failed;
	reporter->show_pass = &assert_passed;
	reporter->show_incomplete = &testcase_failed_to_complete;
	reporter->finish_test = &cute_reporter_testcase_finished;
	reporter->finish_suite = &cute_reporter_suite_finished;
	reporter->memo = memo;

    return reporter;
}

static void cute_reporter_suite_started(TestReporter *reporter, const char *name, const int number_of_tests) {
	// TODO Send number of tests as a parameter
	// int number_of_tests = 2;
	CuteMemo *memo = (CuteMemo *)reporter->memo;
	reporter_start(reporter, name);
	memo->printer("#beginning %s %d\n", name, number_of_tests);
}

static void cute_reporter_testcase_started(TestReporter *reporter, const char *name) {
	CuteMemo *memo = (CuteMemo *)reporter->memo;
	memo->error_count = reporter->failures + reporter->exceptions;
	memo->previous_error = 0;
	reporter_start(reporter, name);
	memo->printer("#starting %s\n", name);
}

static void cute_reporter_testcase_finished(TestReporter *reporter, const char *name) {
	CuteMemo *memo = (CuteMemo *)reporter->memo;
	reporter_finish(reporter, name);
	if (memo->error_count == reporter->failures + reporter->exceptions) {
		memo->printer("#success %s OK\n", name);
	}
}

static void cute_reporter_suite_finished(TestReporter *reporter, const char *name) {
	CuteMemo *memo = (CuteMemo *)reporter->memo;
	reporter_finish(reporter, name);
	memo->printer("#ending %s", name);
	if (get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb) == 0) {
		memo->printer(": %d pass%s, %d failure%s, %d exception%s.\n",
				reporter->passes,
				reporter->passes == 1 ? "" : "es",
				reporter->failures,
				reporter->failures == 1 ? "" : "s",
				reporter->exceptions,
				reporter->exceptions == 1 ? "" : "s");
	} else
		memo->printer("\n");
}

static void assert_failed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
	CuteMemo *memo = (CuteMemo *)reporter->memo;
	if (!memo->previous_error) {
		char buffer[1000];
		memo->printer("#failure %s", get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb));
		memo->printer(" %s:%d ", file, line);
		vsprintf(buffer, (message == NULL ? "Problem" : message), arguments);
		memo->printer("%s\n", buffer);
		memo->previous_error = 1;
	}
}

static void assert_passed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
}

static void testcase_failed_to_complete(TestReporter *reporter, const char *name) {
	CuteMemo *memo = (CuteMemo *)reporter->memo;
    memo->printer("#error %s failed to complete\n", name);
}

/* vim: set ts=4 sw=4 et cindent: */
