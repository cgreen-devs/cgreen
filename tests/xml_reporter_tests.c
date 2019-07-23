#include <cgreen/cgreen.h>
#include <cgreen/breadcrumb.h>
#include <cgreen/messaging.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif


#include "xml_reporter_internal.h"


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

static char* pointer_to_substring(char* haystack, const char* needle) {
    return strstr(haystack,needle);
}

static char* pointer_to_second_substring(char* haystack, const char* needle) {
    return strstr(strstr(haystack,needle)+1,needle);
}

static int mocked_printf(FILE *file, const char *format, ...) {
    char buffer[10000];
    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);

    (void)file;
    output = concat(output, buffer);
    return strlen(output);
}

static TestReporter *reporter;

static void setup_xml_reporter_tests(void) {
    reporter = create_xml_reporter("PREFIX");

    // We can not use setup_reporting() since we are running
    // inside a test suite which needs the real reporting
    // So we'll have to set up the messaging explicitly
    reporter->ipc = start_cgreen_messaging(667);

    clear_output();
    set_xml_reporter_printer(reporter, mocked_printf);
}

static void teardown_xml_reporter_tests(void) {
    //bad mojo when running tests in same process, as destroy_reporter also sets
    //context.reporter = NULL, thus breaking the next test to run
    destroy_reporter(reporter);
    if (NULL != output) {
        free(output);
        //need to set output to NULL to avoid second free in
        //subsequent call to setup_xml_reporter_tests->clear_output
        //when running tests in same process
        output = NULL;
    }
}


Describe(XmlReporter);
BeforeEach(XmlReporter) {
    setup_xml_reporter_tests();
}
AfterEach(XmlReporter) {
    teardown_xml_reporter_tests();
}


Ensure(XmlReporter, will_report_beginning_of_suite) {
    reporter->start_suite(reporter, "suite_name", 2);
    assert_that(output, begins_with_string("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n<testsuite name=\"suite_name"));
}


Ensure(XmlReporter, will_report_beginning_and_successful_finishing_of_passing_test) {
    va_list null_arguments;
    memset(&null_arguments, 0, sizeof(null_arguments));

    reporter->start_test(reporter, "test_name");
    assert_that(output, begins_with_string("<testcase classname=\"\""));
    assert_that(output, contains_string("name=\"test_name\""));

    clear_output();

    reporter->show_pass(reporter, "file", 2, "test_name", null_arguments);
    assert_that(strlen(output), is_equal_to(0));

    reporter->finish_test(reporter, "filename", line, NULL);
    assert_that(output, contains_string("</testcase>"));
}


Ensure(XmlReporter, will_report_a_failing_test) {
    va_list null_arguments;
    memset(&null_arguments, 0, sizeof(null_arguments));

    reporter->start_test(reporter, "test_name");
    reporter->show_fail(reporter, "file", 2, "test_name", null_arguments);
    reporter->finish_test(reporter, "filename", line, NULL);

    assert_that(output, contains_string("<failure message=\"test_name\">"));
    assert_that(output, contains_string("<location file=\"file\" line=\"2\"/>"));
    assert_that(strstr(output, "time="), is_less_than(strstr(output, "<failure")));
}

Ensure(XmlReporter, will_report_a_failing_test_only_once) {
    va_list null_arguments;
    memset(&null_arguments, 0, sizeof(null_arguments));

    reporter->start_test(reporter, "test_name");
    reporter->show_fail(reporter, "file", 2, "test_failure_message", null_arguments);
    reporter->show_fail(reporter, "file", 2, "other_message", null_arguments);
    reporter->finish_test(reporter, "filename", line, NULL);

    assert_that(pointer_to_substring(output, "<failure message=\"test_failure_message\">"), is_not_null);
    assert_that(pointer_to_substring(output, "<failure message=\"other_message\">"), is_not_null);
    assert_that(pointer_to_second_substring(output, "<failure message=\"test_failure_message\">"), is_null);
}


Ensure(XmlReporter, will_report_finishing_of_suite) {
    reporter->start_suite(reporter, "suite_name", 1);
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, contains_string("</testsuite>"));
}


Ensure(XmlReporter, will_mark_ignored_test_as_skipped) {
    const int line = 666;

    reporter->start_suite(reporter, "suite_name", 1);
    reporter->start_test(reporter, "skipped_test_name");
    send_reporter_skipped_notification(reporter);
    reporter->finish_test(reporter, "filename", line, "message");
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, contains_string("<skipped />"));
    assert_that(strstr(output, "time="), is_less_than(strstr(output, "<skipped")));
}


Ensure(XmlReporter, will_report_non_finishing_test) {
    const int line = 666;

    reporter->start_suite(reporter, "suite_name", 1);
    reporter->start_test(reporter, "test_name");
    send_reporter_exception_notification(reporter);
    reporter->finish_test(reporter, "filename", line, "message");
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, contains_string("error type=\"Fatal\""));
    assert_that(output, contains_string("message=\"message\""));
}

Ensure(XmlReporter, will_report_time_correctly_for_non_finishing_test) {
    const int line = 666;

    reporter->start_suite(reporter, "suite_name", 1);
    reporter->start_test(reporter, "test_name");
    send_reporter_exception_notification(reporter);
    reporter->finish_test(reporter, "filename", line, "message");
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, contains_string("name=\"test_name\" time=\""));
}


TestSuite *xml_reporter_tests(void) {
    TestSuite *suite = create_test_suite();
    set_setup(suite, setup_xml_reporter_tests);

    add_test_with_context(suite, XmlReporter, will_report_beginning_of_suite);
    add_test_with_context(suite, XmlReporter, will_report_beginning_and_successful_finishing_of_passing_test);
    add_test_with_context(suite, XmlReporter, will_report_a_failing_test);
    add_test_with_context(suite, XmlReporter, will_report_a_failing_test_only_once);
    add_test_with_context(suite, XmlReporter, will_mark_ignored_test_as_skipped);
    add_test_with_context(suite, XmlReporter, will_report_finishing_of_suite);
    add_test_with_context(suite, XmlReporter, will_report_non_finishing_test);
    add_test_with_context(suite, XmlReporter, will_report_time_correctly_for_non_finishing_test);

    set_teardown(suite, teardown_xml_reporter_tests);
    return suite;
}
