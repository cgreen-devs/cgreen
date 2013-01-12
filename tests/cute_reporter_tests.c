#include <cgreen/breadcrumb.h>
#include <cgreen/cgreen.h>
#include <cgreen/cute_reporter.h>
#include <cgreen/messaging.h>

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static char *output = NULL;

static void clear_output()
{
    if (NULL != output) {
        free(output);
    }

    output = (char*)malloc(1);
    *output = '\0';
}

static char *concat(char *output, char *buffer) {
	output = (char *) realloc(output, strlen(output)+strlen(buffer)+1);
	strcat(output, buffer);
	return output;
}

static int mocked_printf(const char *format, ...) {
	char buffer[10000];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);

	output = concat(output, buffer);
	return strlen(output);
}

TestReporter *reporter;

static void setup_cute_reporter_tests() {
    output = NULL;
    reporter = create_cute_reporter();

    // We can not use setup_reporting() since we are running
    // inside an test suite which needs the real reporting
    // So we'll have to set up the messaging explicitly
    reporter->ipc = start_cgreen_messaging(666);

    clear_output();
    set_cute_printer(reporter, mocked_printf);
}

static void cute_reporter_tests_teardown() {
    destroy_reporter(reporter);
    if (NULL != output) {
        free(output);
    }
}

static void assert_no_output() {
	assert_that(strlen(output), is_equal_to(0));
}

Ensure(will_report_beginning_of_suite) {
	reporter->start_suite(reporter, "suite_name", 2);
	assert_that(output, begins_with_string("#beginning"));
	assert_that(output, contains_string("suite_name"));
}

Ensure(will_report_beginning_and_successful_finishing_of_test) {
	reporter->start_test(reporter, "test_name");
	assert_that(output, begins_with_string("#starting"));
	assert_that(output, contains_string("test_name"));

	clear_output();

#if (defined(__CYGWIN__) && __GNUC__ == 4 && __GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ == 3) \
 || (defined(__linux__) && __GNUC__ == 4 && __GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ == 3)
	va_list arguments = NULL;
#else
	va_list arguments;
#endif
	reporter->show_pass(reporter, "file", 2, "test_name", arguments);
	assert_no_output();

	// Must indicate test case completion before calling finish_test()
	send_reporter_completion_notification(reporter);
	const int line = 666;
	reporter->finish_test(reporter, "filename", line);
	assert_that(output, begins_with_string("#success"));
	assert_that(output, contains_string("test_name"));
}

Ensure(will_report_failing_of_test_only_once) {
	reporter->start_test(reporter, "test_name");

	clear_output();
	reporter->failures++;	// Simulating a failed assert

#if (defined(__CYGWIN__) && __GNUC__ == 4 && __GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ == 3) \
 || (defined(__linux__)  && __GNUC__ == 4 && __GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ == 3)
	va_list arguments = NULL;
#else
	va_list arguments;
#endif
	reporter->show_fail(reporter, "file", 2, "test_name", arguments);
	assert_that(output, begins_with_string("#failure"));
	assert_that(output, contains_string("test_name"));

	clear_output();
	reporter->failures++;	// Simulating another failed assert
	reporter->show_fail(reporter, "file", 2, "test_name", arguments);
	assert_no_output();

	// Must indicate test case completion before calling finish_test()
	send_reporter_completion_notification(reporter);
	const int line = 666;
	reporter->finish_test(reporter, "filename", line);
	assert_no_output();
}

Ensure(will_report_finishing_of_suite) {
	// Must indicate test suite completion before calling finish_suite()
	const int line = 666;
	reporter_start(reporter, "suite_name");
//	reporter->start_suite(reporter, "filename", line);
	send_reporter_completion_notification(reporter);
	reporter->finish_suite(reporter, "filename", line);
	assert_that(output, begins_with_string("#ending"));
	assert_that(output, contains_string("suite_name"));
}

TestSuite *cute_reporter_tests() {
	TestSuite *suite = create_test_suite();
	set_setup(suite, setup_cute_reporter_tests);

	add_test(suite, will_report_beginning_of_suite);
	add_test(suite, will_report_beginning_and_successful_finishing_of_test);
	add_test(suite, will_report_failing_of_test_only_once);
	add_test(suite, will_report_finishing_of_suite);

	set_teardown(suite, cute_reporter_tests_teardown);
	return suite;
}
