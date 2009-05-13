#include <cgreen/text_reporter.h>
#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>

static void text_reporter_start_suite(TestReporter *reporter, const char *name, const int number_of_tests);
static void text_reporter_start_test(TestReporter *reporter, const char *name);
static void text_reporter_finish(TestReporter *reporter, const char *name);
static void show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *name);
static void show_breadcrumb(const char *name, void *memo);

TestReporter *create_text_reporter() {
    TestReporter *reporter = create_reporter();
	reporter->start_suite = &text_reporter_start_suite;
	reporter->start_test = &text_reporter_start_test;
	reporter->show_fail = &show_fail;
	reporter->show_incomplete = &show_incomplete;
	reporter->finish_test = &text_reporter_finish;
	reporter->finish_suite = &text_reporter_finish;
    return reporter;
}

static void text_reporter_start_suite(TestReporter *reporter, const char *name, const int number_of_tests) {
	reporter_start(reporter, name);
	if (get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb) == 1) {
		printf("Running \"%s\"...\n",
		       get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb));
	}
}

static void text_reporter_start_test(TestReporter *reporter, const char *name) {
	reporter_start(reporter, name);
}

static void text_reporter_finish(TestReporter *reporter, const char *name) {
	reporter_finish(reporter, name);
	if (get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb) == 0) {
		printf(
				"Completed \"%s\": %d pass%s, %d failure%s, %d exception%s.\n",
				name,
				reporter->passes,
				reporter->passes == 1 ? "" : "es",
				reporter->failures,
				reporter->failures == 1 ? "" : "s",
				reporter->exceptions,
				reporter->exceptions == 1 ? "" : "s");
	}
}

static void show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    int i = 0;
    printf("%s:%d - Failure!: ", file, line);
    walk_breadcrumb(
            (CgreenBreadcrumb *)reporter->breadcrumb,
            &show_breadcrumb,
            (void *)&i);
    vprintf((message == NULL ? "Problem" : message), arguments);
    printf(" at [%s] line [%d]\n", file, line);
}

static void show_incomplete(TestReporter *reporter, const char *name) {
    int i = 0;
    printf("Exception!: ");
    walk_breadcrumb(
            (CgreenBreadcrumb *)reporter->breadcrumb,
            &show_breadcrumb,
            (void *)&i);
    printf("Test \"%s\" failed to complete\n", name);
}

static void show_breadcrumb(const char *name, void *memo) {
    if (*(int *)memo > 0) {
        printf("%s -> ", name);
    }
    (*(int *)memo)++;
}

/* vim: set ts=4 sw=4 et cindent: */
