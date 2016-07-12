#include <cgreen/breadcrumb.h>
#include <cgreen/cgreen.h>
#include <cgreen/messaging.h>

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
using namespace cgreen;
#endif


#include <cgreen/text_reporter.h>
#include "src/text_reporter_internal.h"



static const int line=666;
static const uint32_t duration_in_milliseconds = 1;
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

static int mocked_printer(const char *format, ...) {
    char buffer[10000];
    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);

    output = concat(output, buffer);
    return strlen(output);
}

static TestReporter *reporter;


static void text_reporter_tests_setup() {
    reporter = create_text_reporter();

    // We can not use setup_reporting() since we are running
    // inside a test suite which needs the real reporting
    // So we'll have to set up the messaging explicitly
    reporter->ipc = start_cgreen_messaging(666);

    clear_output();
    set_text_reporter_printer(reporter, mocked_printer);
}

static void text_reporter_tests_teardown() {
    reporter->destroy(reporter);

    // bad mojo when running tests in same process, as destroy_reporter
    // also sets context.reporter = NULL, thus breaking the next test
    // to run
    if (output != NULL) {
        free(output);
        // need to set output to NULL to avoid second free in
        // subsequent call to setup_*_reporter_tests->clear_output when
        // running tests in same process
        output = NULL;
    }
}


Describe(TextReporter);
BeforeEach(TextReporter) {
    text_reporter_tests_setup();
}
AfterEach(TextReporter) {
    text_reporter_tests_teardown();
}

Ensure(TextReporter, will_report_beginning_and_end_of_suites) {
    reporter->start_suite(reporter, "suite_name", 2);
    reporter->finish_suite(reporter, "filename", line, 0);
    assert_that(output, begins_with_string("Running \"suite_name\" (2 tests)"));
    assert_that(output, contains_string("Completed \"suite_name\""));
}


Ensure(TextReporter, will_report_passed_for_test_with_one_pass_on_completion) {
    reporter->start_suite(reporter, "suite_name", 15);
    reporter->start_test(reporter, "test_name");

    (*reporter->assert_true)(reporter, "file", 2, true, "");

    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL, duration_in_milliseconds);
    reporter->finish_suite(reporter, "filename", line, duration_in_milliseconds);

    assert_that(output, contains_string("Completed \"suite_name\": 1 pass"));
}


Ensure(TextReporter, will_report_failed_once_for_each_fail) {
    va_list arguments;

    reporter->start_test(reporter, "test_name");

    reporter->failures++;   // Simulating a failed assert
    memset(&arguments, 0, sizeof(va_list));
    reporter->show_fail(reporter, "file", 2, "test_name", arguments);

    assert_that(output, contains_string("file:2: Failure"));
    clear_output();

    reporter->failures++;   // Simulating another failed assert
    reporter->show_fail(reporter, "file", 3, "test_name", arguments);
    assert_that(output, contains_string("file:3: Failure:"));
    clear_output();
    
    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL, duration_in_milliseconds);
    assert_that(output, is_equal_to_string(""));
}


Ensure(TextReporter, will_report_non_finishing_test) {
    const int line = 666;
    reporter->start_suite(reporter, "suite_name", 1);

    send_reporter_exception_notification(reporter);
    reporter->finish_suite(reporter, "filename", line, duration_in_milliseconds);

    assert_that(output, contains_string("1 exception"));
}

TestSuite *text_reporter_tests() {
    TestSuite *suite = create_test_suite();
    set_setup(suite, text_reporter_tests_setup);

    add_test_with_context(suite, TextReporter, will_report_beginning_and_end_of_suites);
    add_test_with_context(suite, TextReporter, will_report_passed_for_test_with_one_pass_on_completion);
    add_test_with_context(suite, TextReporter, will_report_failed_once_for_each_fail);
    add_test_with_context(suite, TextReporter, will_report_non_finishing_test);
    
    set_teardown(suite, text_reporter_tests_teardown);
    return suite;
}
