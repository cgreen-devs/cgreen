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
		int line, const char *message, uint32_t duration_in_milliseconds);
static void show_fail(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line,
		const char *message, va_list arguments);
static void show_breadcrumb(const char *name, void *memo);
static void text_reporter_finish_suite(TestReporter *reporter, const char *file, int line,
									   uint32_t duration_in_milliseconds);

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
		printf("Running \"%s\" (%d tests)%s",
				get_current_from_breadcrumb(
						(CgreenBreadcrumb *) reporter->breadcrumb),
               number_of_tests,
               ((TextReporterOptions *)reporter->options)->quiet_mode?":":"...\n");
        fflush(stdout);
	}
}

static void text_reporter_start_test(TestReporter *reporter, const char *name) {
	reporter_start(reporter, name);
}

static void text_reporter_finish(TestReporter *reporter, const char *filename,
		int line, const char *message, uint32_t duration_in_milliseconds) {
	reporter_finish(reporter, filename, line, message, duration_in_milliseconds);
}


static void format_count(char *buff, int count, const char *name, const char *color, const char *plural, bool use_colors) {
    const char *color_on = use_colors?color:"";
    const char *color_off = use_colors?RESET:"";
    sprintf(buff, "%s%d %s%s%s", count>0?color_on:"", count, name, count==1?"":plural, color_off);
}

static char *format_passes(int passes, bool use_colors) {
    static char buff[100];
    format_count(buff, passes, "pass", GREEN, "es", use_colors);
    return buff;
}

static char *format_failures(int failures, bool use_colors) {
    static char buff[100];
    format_count(buff, failures, "failure", RED, "s", use_colors);
    return buff;
}

static char *format_exceptions(int exceptions, bool use_colors) {
    static char buff[100];
    format_count(buff, exceptions, "exception", MAGENTA, "s", use_colors);
    return buff;
}

static void text_reporter_finish_suite(TestReporter *reporter, const char *file, int line, uint32_t duration_in_milliseconds) {
	const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb);
    bool use_colors = reporter->options && ((TextReporterOptions *)reporter->options)->use_colours;

    reporter_finish_suite(reporter, file, line, duration_in_milliseconds);

    if (((TextReporterOptions *)reporter->options)->quiet_mode) {
        printf(".");
        if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 0)
            printf("\n");
    } else {
        printf("Completed \"%s\": %s, %s, %s in %dms.\n",
               name,
               format_passes(reporter->passes, use_colors),
               format_failures(reporter->failures, use_colors),
               format_exceptions(reporter->exceptions, use_colors),
               duration_in_milliseconds);
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
