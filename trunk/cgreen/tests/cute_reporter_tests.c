#include <cgreen/cgreen.h>
#include <cgreen/cute_reporter.h>
#include <cgreen/messaging.h>

#include <stdio.h>
#include <string.h>

static char *output;
static void clear_output()
{
    output = (char*)malloc(1);
    *output = '\0';
}

static char *concat(char *output, char *buffer) {
	output = realloc(output, strlen(output)+strlen(buffer)+1);
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

int strpos(char *whole, char *part) {
	int pos;
	if (strlen(whole) >= strlen(part))
		for (pos = 0; pos < strlen(whole)-strlen(part); pos++) {
			if (strncmp(&whole[pos], part, strlen(part)) == 0)
				return pos;
		}
	return -1;
}

TestReporter *reporter;

static void setup_cute_reporter_tests() {
	reporter = create_cute_reporter();

	// We can not use setup_reporting() since we are running
	// inside an test suite which needs the real reporting
	// So we'll have to set up the messaging explicitly
	reporter->ipc = start_cgreen_messaging(666);

	clear_output();
	set_cute_printer(reporter, mocked_printf);
}


static void assert_no_output() {
    assert_equal(strlen(output), 0);
}

static void assert_output_starts_with(char *string) {
	assert_equal(strpos(output, string), 0);
}

static void assert_output_contains(char *string) {
	assert_true(strpos(output, string) > 0);
}

Ensure will_report_beginning_of_suite() {
	reporter->start_suite(reporter, "suite_name", 2);
	assert_output_starts_with("#beginning");
	assert_output_contains("suite_name");
}

Ensure will_report_beginning_and_successful_finishing_of_test() {
	reporter->start_test(reporter, "test_name");
	assert_output_starts_with("#starting");
	assert_output_contains("test_name");

	clear_output();
	reporter->show_pass(reporter, "file", 2, "test_name", "");
	assert_no_output();

	// Must indicate test case completion before calling finish_test()
	send_reporter_completion_notification(reporter);
	reporter->finish_test(reporter, "test_name");
	assert_output_starts_with("#success");
	assert_output_contains("test_name");
}

Ensure will_report_failing_of_test_only_once() {
	reporter->start_test(reporter, "test_name");

	clear_output();
	reporter->failures++;	// Simulating a failed assert
	reporter->show_fail(reporter, "file", 2, "test_name", "");
	assert_output_starts_with("#failure");
	assert_output_contains("test_name");

	clear_output();
	reporter->failures++;	// Simulating another failed assert
	reporter->show_fail(reporter, "file", 2, "test_name", "");
	assert_no_output();

	// Must indicate test case completion before calling finish_test()
	send_reporter_completion_notification(reporter);
	reporter->finish_test(reporter, "test_name");
	assert_no_output();
}

Ensure will_report_finishing_of_suite() {
	// Must indicate test suite completion before calling finish_suite()
	send_reporter_completion_notification(reporter);
	reporter->finish_suite(reporter, "suite_name");
	assert_output_starts_with("#ending");
	assert_output_contains("suite_name");
}

TestSuite *cute_reporter_tests() {
	TestSuite *suite = create_test_suite();
    setup(suite, setup_cute_reporter_tests);

	add_test(suite, will_report_beginning_of_suite);
	add_test(suite, will_report_beginning_and_successful_finishing_of_test);
	add_test(suite, will_report_failing_of_test_only_once);
	add_test(suite, will_report_finishing_of_suite);
	return suite;
}
