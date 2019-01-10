
#include <cgreen/breadcrumb.h>
#include <cgreen/cgreen.h>
#include <cgreen/messaging.h>

#include <stdio.h>
#include <string.h>

#include <cgreen/cute_reporter.h>
#include "cute_reporter_internal.h"


#ifdef __cplusplus
using namespace cgreen;
#endif

static const int line=666;
static char *output = NULL;

static void clear_output(void)
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

static int mocked_vprintf(const char *format, va_list arguments) {
    char buffer[10000];

    vsprintf(buffer, format, arguments);

    output = concat(output, buffer);
    return strlen(output);
}

static int mocked_printf(const char *format, ...) {
   int result = 0;

   va_list ap;
   va_start(ap, format);
   result = mocked_vprintf(format, ap);
   va_end(ap);

   return result;
}

static TestReporter *reporter;

static void setup_cute_reporter_tests(void) {
    reporter = create_cute_reporter();

    // We can not use setup_reporting() since we are running
    // inside a test suite which needs the real reporting
    // So we'll have to set up the messaging explicitly
    reporter->ipc = start_cgreen_messaging(666);

    clear_output();
    set_cute_reporter_printer(reporter, mocked_printf);
    set_cute_reporter_vprinter(reporter, mocked_vprintf);
}

static void teardown_cute_reporter_tests(void) {
    //bad mojo when running tests in same process, as destroy_reporter also sets
    //context.reporter = NULL, thus breaking the next test to run
    destroy_reporter(reporter);
    if (NULL != output) {
        free(output);
        //need to set output to NULL to avoid second free in subsequent call to setup_cute_reporter_tests->clear_output
        //when running tests in same process
        output = NULL;
    }
}

static void assert_no_output(void) {
    assert_that(strlen(output), is_equal_to(0));
}

Describe(CuteReporter);
BeforeEach(CuteReporter) {
    setup_cute_reporter_tests();
}
AfterEach(CuteReporter) {
    teardown_cute_reporter_tests();
}

Ensure(CuteReporter, will_report_beginning_of_suite) {
    reporter->start_suite(reporter, "suite_name", 2);
    assert_that(output, begins_with_string("#beginning"));
    assert_that(output, contains_string("suite_name"));
}

Ensure(CuteReporter, will_report_beginning_and_successful_finishing_of_test) {
    va_list arguments;

    reporter->start_test(reporter, "test_name");
    assert_that(output, begins_with_string("#starting"));
    assert_that(output, contains_string("test_name"));

    clear_output();

    memset(&arguments, 0, sizeof(va_list));
    reporter->show_pass(reporter, "file", 2, "test_name", arguments);
    assert_no_output();

    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL);
    assert_that(output, begins_with_string("#success"));
    assert_that(output, contains_string("test_name"));
}

Ensure(CuteReporter, will_report_failing_of_test_only_once) {
    va_list arguments;

    reporter->start_test(reporter, "test_name");

    clear_output();
    reporter->failures++;   // Simulating a failed assert
    memset(&arguments, 0, sizeof(va_list));
    reporter->show_fail(reporter, "file", 2, "test_name", arguments);
    assert_that(output, begins_with_string("#failure"));
    assert_that(output, contains_string("test_name"));

    clear_output();
    reporter->failures++;   // Simulating another failed assert
    reporter->show_fail(reporter, "file", 2, "test_name", arguments);
    assert_no_output();

    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL);
    assert_no_output();
}

static void wrapped_show_fail(TestReporter *reporter, const char *file, int line,
        const char *message, ...) {
   va_list arguments;
   va_start(arguments, message);
   reporter->show_fail(reporter, file, line, message, arguments);
   va_end(arguments);
}

Ensure(CuteReporter, will_use_arguments_for_show_fail) {
    reporter->start_test(reporter, "test_name");

    clear_output();
    reporter->failures++;   // Simulating a failed assert
    wrapped_show_fail(reporter, "file", 2, "test_case %i provides %s", 42, "the answer");
    assert_that(output, begins_with_string("#failure"));
    assert_that(output, contains_string("test_case 42 provides the answer"));

    clear_output();

    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL);
    assert_no_output();
}

Ensure(CuteReporter, will_report_finishing_of_suite) {
    // Must indicate test suite completion before calling finish_suite()
    reporter->start_suite(reporter, "suite_name", 1);

    clear_output();
    
    send_reporter_completion_notification(reporter);
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, begins_with_string("#ending"));
    assert_that(output, contains_string("suite_name"));
}

Ensure(CuteReporter, will_report_non_finishing_test) {
    const int line = 666;
    reporter->start_suite(reporter, "suite_name", 1);
    clear_output();

    send_reporter_exception_notification(reporter);
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, begins_with_string("#error"));
    assert_that(output, contains_string("failed to complete"));
}

TestSuite *cute_reporter_tests(void) {
    TestSuite *suite = create_test_suite();
    set_setup(suite, setup_cute_reporter_tests);

    add_test_with_context(suite, CuteReporter, will_report_beginning_of_suite);
    add_test_with_context(suite, CuteReporter, will_report_beginning_and_successful_finishing_of_test);
    add_test_with_context(suite, CuteReporter, will_report_failing_of_test_only_once);
    add_test_with_context(suite, CuteReporter, will_report_finishing_of_suite);
    add_test_with_context(suite, CuteReporter, will_report_non_finishing_test);

    set_teardown(suite, teardown_cute_reporter_tests);
    return suite;
}
