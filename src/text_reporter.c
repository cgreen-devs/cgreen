#include "text_reporter.h"
#include "reporter.h"
#include <stdlib.h>
#include <stdio.h>

static void text_reporter_start(TestReporter *reporter, char *name);
static void text_reporter_finish(TestReporter *reporter, char *name);
static void show_fail(TestReporter *reporter, const char *file, int line, char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, char *name);
static void show_breadcrumb(char *name, void *memo);

TestReporter *create_text_reporter() {
    TestReporter *reporter = create_reporter();
	reporter->start = &text_reporter_start;
	reporter->finish = &text_reporter_finish;
	reporter->show_fail = &show_fail;
	reporter->show_incomplete = &show_incomplete;
    return reporter;
}

static void text_reporter_start(TestReporter *reporter, char *name) {
	reporter_start(reporter, name);
	if (reporter->breadcrumb_depth == 1) {
		printf("Running \"%s\"...\n", get_current_reporter_test(reporter));
	}
}

static void text_reporter_finish(TestReporter *reporter, char *name) {
	reporter_finish(reporter, name);
	if (reporter->breadcrumb_depth == 0) {
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

static void show_fail(TestReporter *reporter, const char *file, int line, char *message, va_list arguments) {
    int i = 0;
    printf("Failure!: ");
    walk_reporter_breadcrumb(reporter, &show_breadcrumb, (void *)&i);
    vprintf((message == NULL ? "Problem" : message), arguments);
    printf(" at [%s] line [%d]\n", file, line);
}

static void show_incomplete(TestReporter *reporter, char *name) {
    int i = 0;
    printf("Exception!: ");
    walk_reporter_breadcrumb(reporter, &show_breadcrumb, (void *)&i);
    printf("Test \"%s\" failed to complete\n", name);
}

static void show_breadcrumb(char *name, void *memo) {
    if (*(int *)memo > 0) {
        printf("%s -> ", name);
    }
    (*(int *)memo)++;
}
