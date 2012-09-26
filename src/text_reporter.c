#include <cgreen/text_reporter.h>
#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
namespace cgreen {
#endif

static void text_reporter_start_suite(TestReporter *reporter, const char *name,
		const int number_of_tests);
static void text_reporter_start_test(TestReporter *reporter, const char *name);
static void text_reporter_finish(TestReporter *reporter, const char *filename,
		int line);
static void show_fail(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments);
static void show_breadcrumb(const char *name, void *memo);

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
	reporter->finish_suite = &text_reporter_finish;
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
		int line) {
	const char *name = get_current_from_breadcrumb(
			(CgreenBreadcrumb *) reporter->breadcrumb);

	reporter_finish(reporter, filename, line);
	if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 0) {
		printf("Completed \"%s\": %d pass%s, %d failure%s, %d exception%s.\n",
				name, reporter->passes, reporter->passes == 1 ? "" : "es",
				reporter->failures, reporter->failures == 1 ? "" : "s",
				reporter->exceptions, reporter->exceptions == 1 ? "" : "s");
	}
}

static void show_fail(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments) {
	int i = 0;
	printf("%s:%d: ", file, line);
	printf("Failure: ");
	walk_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, &show_breadcrumb,
			(void *) &i);
	printf("\n\t");
	vprintf((message == NULL ? "<NULL for failure message>\n" : message), arguments);
    fflush(NULL);
}

static void show_incomplete(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments) {
	int i = 0;
	printf("%s:%d: ", file, line);
	printf("Exception: ");
	walk_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, &show_breadcrumb,
			(void *) &i);

	printf("\n");
	vprintf((message == NULL ? "\tTest terminated unexpectedly, likely from a non-standard exception, SIGSEGV, or other signal" : message), arguments);
	printf("\n");
    fflush(NULL);
}

static void show_breadcrumb(const char *name, void *memo) {
	if (*(int *) memo > 0) {
		printf("-> %s ", name);
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
