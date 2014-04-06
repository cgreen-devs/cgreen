#include <cgreen/text_reporter.h>
#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"


#ifdef __cplusplus
namespace cgreen {
#endif

static void text_reporter_start_suite(TestReporter *reporter, const char *name,
		const int number_of_tests);
static void text_reporter_start_test(TestReporter *reporter, const char *name);
static void text_reporter_finish(TestReporter *reporter, const char *filename,
		int line, const char *message);
static void show_fail(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments);
static void show_breadcrumb(const char *name, void *memo);
static void text_reporter_finish_suite(TestReporter *reporter, const char *file, int line);

TestReporter *create_text_reporter(void) {
	TestReporter *reporter = create_reporter();
	if (reporter == NULL) {
		return NULL;
	}
	reporter->start_suite = &text_reporter_start_suite;
	reporter->start_test = &text_reporter_start_test;
	reporter->show_fail = &show_fail;
	reporter->show_incomplete = &show_incomplete;
	reporter->finish_test = &text_reporter_finish;
	reporter->finish_suite = &text_reporter_finish_suite;
	return reporter;
}

static void text_reporter_start_suite(TestReporter *reporter, const char *name,
		const int number_of_tests) {
	reporter_start(reporter, name);
	if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 1) {
		printf("Running \"%s\" (%d tests)...\n",
				get_current_from_breadcrumb(
						(CgreenBreadcrumb *) reporter->breadcrumb),
				number_of_tests);
        fflush(stdout);
	}
}

static void text_reporter_start_test(TestReporter *reporter, const char *name) {
	reporter_start(reporter, name);
}

static void text_reporter_finish(TestReporter *reporter, const char *filename,
		int line, const char *message) {
	reporter_finish(reporter, filename, line, message);
}

static void text_reporter_finish_suite(TestReporter *reporter, const char *file, int line) {
	const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb);

    reporter_finish_suite(reporter, file, line);

    if (reporter->options && ((TextReporterOptions *)reporter->options)->use_colours)
        printf("Completed \"%s\": %s%d pass%s%s, %s%d failure%s%s, %s%d exception%s%s.\n",
               name,
			   (reporter->passes > 0) ? GREEN : "", reporter->passes, reporter->passes == 1 ? "" : "es", RESET, 
               (reporter->failures > 0) ? RED : "", reporter->failures, reporter->failures == 1 ? "" : "s", RESET,
               (reporter->exceptions > 0) ? MAGENTA : "", reporter->exceptions, reporter->exceptions == 1 ? "" : "s", RESET);
    else
        printf("Completed \"%s\": %d pass%s, %d failure%s, %d exception%s.\n",
               name, reporter->passes, reporter->passes == 1 ? "" : "es",
               reporter->failures, reporter->failures == 1 ? "" : "s",
               reporter->exceptions, reporter->exceptions == 1 ? "" : "s");
}

static void show_fail(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments) {
	int i = 0;
	printf("%s:%d: ", file, line);
	printf("Failure: ");
	walk_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, &show_breadcrumb,
			(void *) &i);
	printf("\n\t");
	vprintf((message == NULL ? "<NULL for failure message>" : message), arguments);
	printf("\n");
	printf("\n");
    fflush(NULL);
}

static void show_incomplete(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments) {
	int i = 0;
	printf("%s:%d: ", file, line);
	printf("Exception: ");
	walk_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, &show_breadcrumb,
			(void *) &i);

	printf("\n\t");
	vprintf(message ? message: "Test terminated unexpectedly, likely from a non-standard exception or Posix signal", arguments);
	printf("\n");
	printf("\n");
    fflush(NULL);
}

static void show_breadcrumb(const char *name, void *memo) {
	if (*(int *) memo > 1) {
		printf("-> ");
	}
	if (*(int *) memo > 0) {
		printf("%s ", name);
	}
	(*(int *) memo)++;
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
/* Local variables: */
/* tab-width: 4     */
/* End:             */
