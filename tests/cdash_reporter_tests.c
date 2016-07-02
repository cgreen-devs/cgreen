#include <cgreen/breadcrumb.h>
#include <cgreen/cgreen.h>
#include <cgreen/messaging.h>

#include <stdio.h>
#include <string.h>

#include <cgreen/cdash_reporter.h>
#include "src/cdash_reporter_internal.h"

#ifdef __cplusplus
using namespace cgreen;
#endif

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

static int mocked_printer(FILE *stream, const char *format, ...) {
    char buffer[10000];
    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);

    (void)stream;               /* Unused */
    
    output = concat(output, buffer);
    return strlen(output);
}

static TestReporter *reporter;

static CDashInfo info;

static void setup_cdash_reporter_tests() {
    reporter = create_cdash_reporter(&info);

    // We can not use setup_reporting() since we are running
    // inside a test suite which needs the real reporting
    // So we'll have to set up the messaging explicitly
    reporter->ipc = start_cgreen_messaging(666);

    clear_output();
    set_cdash_reporter_printer(reporter, mocked_printer);
}

static void teardown_cdash_reporter_tests() {
    reporter->destroy(reporter);

    //bad mojo when running tests in same process, as destroy_reporter also sets
    //context.reporter = NULL, thus breaking the next test to run
    if (NULL != output) {
        free(output);
        //need to set output to NULL to avoid second free in subsequent call to setup_cdash_reporter_tests->clear_output
        //when running tests in same process
        output = NULL;
    }
}


Describe(CDashReporter);
BeforeEach(CDashReporter) {
    setup_cdash_reporter_tests();
}
AfterEach(CDashReporter) {
    teardown_cdash_reporter_tests();
}

Ensure(CDashReporter, will_report_nothing_for_suites) {
    reporter->start_suite(reporter, "suite_name", 2);
    reporter->finish_suite(reporter, "filename", line, 0);
}

Ensure(CDashReporter, will_report_passed_for_test_with_one_pass) {
    va_list arguments;

    reporter->start_test(reporter, "test_name");

    memset(&arguments, 0, sizeof(va_list));
    reporter->show_pass(reporter, "file", 2, "test_name", arguments);

    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL, duration_in_milliseconds);

    assert_that(output, contains_string("<Test Status=\"passed\">"));
}

Ensure(CDashReporter, will_report_failed_once_for_each_fail) {
    va_list arguments;

    reporter->start_test(reporter, "test_name");

    reporter->failures++;   // Simulating a failed assert
    memset(&arguments, 0, sizeof(va_list));
    reporter->show_fail(reporter, "file", 2, "test_name", arguments);

    assert_that(output, contains_string("<Test Status=\"failed\">"));
    assert_that(output, contains_string("<Name>test_name</Name>"));
    assert_that(output, contains_string("<FullName>file</FullName>"));
    assert_that(output, contains_string("<FullCommandLine>at [file] line [2]</FullCommandLine>"));

    clear_output();

    reporter->failures++;   // Simulating another failed assert
    reporter->show_fail(reporter, "file", 3, "test_name", arguments);
    assert_that(output, contains_string("<Test Status=\"failed\">"));
    assert_that(output, contains_string("<Name>test_name</Name>"));
    assert_that(output, contains_string("<FullCommandLine>at [file] line [3]</FullCommandLine>"));

    clear_output();
    
    // Must indicate test case completion before calling finish_test()
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL, duration_in_milliseconds);
    assert_that(output, is_equal_to_string(""));
}

Ensure(CDashReporter, will_report_non_finishing_test) {
    const int line = 666;
    reporter->start_suite(reporter, "suite_name", 1);

    send_reporter_exception_notification(reporter);
    reporter->finish_suite(reporter, "filename", line, duration_in_milliseconds);

    assert_that(output, contains_string("<Test Status=\"incomplete\">"));
}

TestSuite *cdash_reporter_tests() {
    TestSuite *suite = create_test_suite();
    set_setup(suite, setup_cdash_reporter_tests);

    add_test_with_context(suite, CDashReporter, will_report_nothing_for_suites);
    add_test_with_context(suite, CDashReporter, will_report_passed_for_test_with_one_pass);
    add_test_with_context(suite, CDashReporter, will_report_failed_once_for_each_fail);
    add_test_with_context(suite, CDashReporter, will_report_non_finishing_test);
    
    set_teardown(suite, teardown_cdash_reporter_tests);
    return suite;
}
