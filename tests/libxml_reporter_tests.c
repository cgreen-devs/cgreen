#include <cgreen/cgreen.h>
#include <cgreen/breadcrumb.h>
#include <cgreen/messaging.h>
#include "cgreen_value_internal.h"
#include "constraint_internal.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <libxml/tree.h>

#ifdef __cplusplus
using namespace cgreen;
#endif


#include "libxml_reporter_internal.h"

#define XMLSTRING(x) (BAD_CAST x)

static const int line=666;
static xmlChar *output = NULL;

static void clear_output(void)
{
    if (NULL != output) {
        xmlFree(output);
    }
    output = NULL;
}

static int mocked_xmlout(xmlDocPtr doc) {
    if (output) {
        xmlFree(output);
    }

    xmlDocDumpMemoryEnc(doc, &output, NULL, "UTF-8");
    return 0;
}

static TestReporter *reporter;

static void setup_xml_reporter_tests(void) {
    reporter = create_libxml_reporter("PREFIX");

    // We can not use setup_reporting() since we are running
    // inside a test suite which needs the real reporting
    // So we'll have to set up the messaging explicitly
    reporter->ipc = start_cgreen_messaging(667);

    clear_output();
    set_libxml_reporter_printer(reporter, mocked_xmlout);
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

static xmlChar* getAttribute(xmlNodePtr node, const xmlChar* name) {
    xmlAttr *attr = node->properties;
    while (attr) {
        if (xmlStrEqual(attr->name, name)) {
            return xmlNodeGetContent((xmlNode*)attr);
        }
        attr = attr->next;
    }

    return NULL;
}

static bool hasAttribute(xmlNodePtr node, const xmlChar* name) {
    xmlAttr *attr = node->properties;
    while (attr) {
        if (xmlStrEqual(attr->name, name)) {
            return true;
        }
        attr = attr->next;
    }

    return false;
}

struct xmlNode_has_attribute_equal_to {
    xmlChar *attr, *value;
};

static bool compare_xmlNode_has_attribute_equal_to(Constraint *constraint,
                                                   CgreenValue actual) {
    xmlNodePtr actualNode =  (xmlNodePtr)actual.value.pointer_value;
    struct xmlNode_has_attribute_equal_to *expected =
        (struct xmlNode_has_attribute_equal_to*)constraint->expected_value.value.pointer_value;
    xmlChar* actualValue = getAttribute(actualNode, expected->attr);

    bool ret = xmlStrEqual(actualValue, expected->value);
    xmlFree(actualValue);
    return ret;
}

static char *failure_message_xmlNode_has_attribute_equal_to(
    Constraint *constraint, const char *actual_string, intptr_t actual_value) {
    struct xmlNode_has_attribute_equal_to *expected =
        (struct xmlNode_has_attribute_equal_to*)constraint->expected_value.value.pointer_value;
    xmlChar* actualValue = getAttribute((xmlNodePtr)actual_value, expected->attr);

    const char *message_template = "Expected attribute [%s] of [%s] to [equal] [%s]\n"
        "\tactual value:\t\t[%s]\n"
        "\texpected to equal:\t[%s]\n";
    size_t msglen = xmlStrlen(expected->attr) + strlen(actual_string) +
        strlen(constraint->expected_value_name) + xmlStrlen(actualValue) +
        xmlStrlen(expected->value) + strlen(message_template);
    char *message = (char*)malloc(msglen);
    if (!message) {
        xmlFree(actualValue);
        return NULL;
    }

    if (snprintf(message, msglen, message_template,
                 expected->attr, actual_string, constraint->expected_value_name,
                 actualValue, expected->value) >= (ssize_t)msglen) {
        xmlFree(actualValue);
        free(message);
        return NULL;
    }

    xmlFree(actualValue);
    return message;
}

static void destroy_xmlNode_has_attribute_equal_to(Constraint* constraint) {
    struct xmlNode_has_attribute_equal_to *expected =
        (struct xmlNode_has_attribute_equal_to*)constraint->expected_value.value.pointer_value;
    xmlFree(expected->attr);
    xmlFree(expected->value);
    free(expected);
    destroy_empty_constraint(constraint);
}

static Constraint *create_xmlNode_has_attribute_equal_to(const xmlChar* attr,
                                                         const xmlChar *value,
                                                         const char* expected_value_name)
{
    Constraint *constraint = create_constraint();
    if (!constraint)
        return NULL;

    struct xmlNode_has_attribute_equal_to *expected =
        (struct xmlNode_has_attribute_equal_to *)malloc(sizeof(struct xmlNode_has_attribute_equal_to));
    if (!expected) {
        constraint->destroy(constraint);
        return NULL;
    }

    expected->attr = xmlStrdup(attr);
    expected->value = xmlStrdup(value);

    constraint->expected_value = make_cgreen_pointer_value(expected);
    constraint->expected_value_name = strdup(expected_value_name);
    constraint->type = CGREEN_STRING_COMPARER_CONSTRAINT;

    constraint->compare = &compare_xmlNode_has_attribute_equal_to;
    constraint->failure_message = &failure_message_xmlNode_has_attribute_equal_to;
    constraint->name = "have attribute with value";
    constraint->size_of_expected_value = sizeof(intptr_t);
    constraint->destroy = &destroy_xmlNode_has_attribute_equal_to;

    return constraint;
}

#define xmlnode_has_attribute_equal_to(attr, value) \
    create_xmlNode_has_attribute_equal_to(XMLSTRING(attr), XMLSTRING(value), #value)

static xmlNodePtr xmlnode_find_sibling(xmlNodePtr node, const char *name)
{
    while (node) {
        if (xmlStrEqual(node->name, XMLSTRING(name)))
            break;
        node = xmlNextElementSibling(node);
    }
    return node;
}

Describe(LibXmlReporter);
BeforeEach(LibXmlReporter) {
    setup_xml_reporter_tests();
}
AfterEach(LibXmlReporter) {
    teardown_xml_reporter_tests();
}


Ensure(LibXmlReporter, will_report_beginning_of_suite) {
    reporter->start_suite(reporter, "suite_name", 2);
    reporter->finish_suite(reporter, "filename", line);

    xmlDocPtr doc = xmlParseDoc(output);
    assert_that(doc, is_not_null);

    xmlNodePtr testsuite = xmlDocGetRootElement(doc);
    assert_that(testsuite->name, is_equal_to_string("testsuite"));
    assert_that(xmlChildElementCount(testsuite), is_equal_to(0));
    assert_that(testsuite, xmlnode_has_attribute_equal_to("name", "suite_name"));
    xmlFreeDoc(doc);
}


static void reporter_show_pass_vargs(TestReporter *reporter, const char *name, int line,
                                     const char *format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    reporter->show_pass(reporter, name, line, format, vargs);
    va_end(vargs);
}

Ensure(LibXmlReporter, will_report_beginning_and_successful_finishing_of_passing_test) {
    reporter->start_suite(reporter, "suite_name", 2);
    reporter->start_test(reporter, "test_name");
    reporter_show_pass_vargs(reporter, "file", 2, "test_name");
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL);
    reporter->finish_suite(reporter, "filename", line);

    xmlDocPtr doc = xmlParseDoc(output);
    assert_that(doc, is_not_null);

    xmlNodePtr testsuite = xmlDocGetRootElement(doc);
    assert_that(xmlChildElementCount(testsuite), is_equal_to(1));

    xmlNodePtr testcase = xmlFirstElementChild(testsuite);
    assert_that(testcase->name, is_equal_to_string("testcase"));
    assert_that(xmlChildElementCount(testcase), is_equal_to(0));
    assert_that(testcase, xmlnode_has_attribute_equal_to("name", "test_name"));
    assert_that(testcase, xmlnode_has_attribute_equal_to("classname", "suite_name"));
}


static void reporter_show_fail_vargs(TestReporter *reporter, const char *name, int line,
                                     const char *format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    reporter->show_fail(reporter, name, line, format, vargs);
    va_end(vargs);
}

Ensure(LibXmlReporter, will_report_a_failing_test) {
    reporter->start_suite(reporter, "suite_name", 2);
    reporter->start_test(reporter, "test_name");
    reporter_show_fail_vargs(reporter, "file", 2, "test_name");
    send_reporter_completion_notification(reporter);
    reporter->finish_test(reporter, "filename", line, NULL);
    reporter->finish_suite(reporter, "filename", line);

    xmlDocPtr doc = xmlParseDoc(output);
    assert_that(doc, is_not_null);

    xmlNodePtr testsuite = xmlDocGetRootElement(doc);
    assert_that(xmlChildElementCount(testsuite), is_equal_to(1));

    xmlNodePtr testcase = xmlFirstElementChild(testsuite);
    assert_that(testcase->name, is_equal_to_string("testcase"));
    assert_that(xmlChildElementCount(testcase), is_equal_to(1));
    assert_that(hasAttribute(testcase, XMLSTRING("time")), is_true);

    xmlNodePtr failure = xmlFirstElementChild(testcase);
    assert_that(failure->name, is_equal_to_string("failure"));
    assert_that(xmlChildElementCount(failure), is_equal_to(1));
    assert_that(failure, xmlnode_has_attribute_equal_to("message", "test_name"));

    xmlNodePtr location = xmlFirstElementChild(failure);
    assert_that(location->name, is_equal_to_string("location"));
    assert_that(xmlChildElementCount(location), is_equal_to(0));
    assert_that(location, xmlnode_has_attribute_equal_to("file", "file"));
    assert_that(location, xmlnode_has_attribute_equal_to("line", "2"));
}

Ensure(LibXmlReporter, will_report_a_failing_test_only_once) {
    va_list null_arguments;
    memset(&null_arguments, 0, sizeof(null_arguments));

    reporter->start_suite(reporter, "suite_name", 2);
    reporter->start_test(reporter, "test_name");
    reporter->show_fail(reporter, "file", 2, "test_failure_message", null_arguments);
    reporter->show_fail(reporter, "file", 2, "other_message", null_arguments);
    reporter->finish_test(reporter, "filename", line, NULL);
    reporter->finish_suite(reporter, "filename", line);

    xmlDocPtr doc = xmlParseDoc(output);
    xmlNodePtr testsuite = xmlDocGetRootElement(doc);
    xmlNodePtr testcase = xmlFirstElementChild(testsuite);

    static const char FAILURE[] = "failure";
    xmlNodePtr failure = xmlnode_find_sibling(xmlFirstElementChild(testcase), FAILURE);

    assert_that(failure->name, is_equal_to_string(FAILURE));
    assert_that(failure, xmlnode_has_attribute_equal_to("message", "test_failure_message"));

    failure = xmlnode_find_sibling(xmlNextElementSibling(failure), FAILURE);
    assert_that(failure, xmlnode_has_attribute_equal_to("message", "other_message"));

    failure = xmlnode_find_sibling(xmlNextElementSibling(failure), FAILURE);
    assert_that(failure, is_null);
}

Ensure(LibXmlReporter, will_mark_ignored_test_as_skipped) {
    const int line = 666;

    reporter->start_suite(reporter, "suite_name", 1);
    reporter->start_test(reporter, "skipped_test_name");
    send_reporter_skipped_notification(reporter);
    reporter->finish_test(reporter, "filename", line, "message");
    reporter->finish_suite(reporter, "filename", line);

    xmlDocPtr doc = xmlParseDoc(output);
    assert_that(doc, is_not_null);

    xmlNodePtr testsuite = xmlDocGetRootElement(doc);
    assert_that(xmlChildElementCount(testsuite), is_equal_to(1));

    xmlNodePtr testcase = xmlFirstElementChild(testsuite);
    assert_that(testcase->name, is_equal_to_string("testcase"));
    assert_that(xmlChildElementCount(testcase), is_equal_to(1));
    assert_that(hasAttribute(testcase, XMLSTRING("time")), is_true);

    xmlNodePtr skipped = xmlFirstElementChild(testcase);
    assert_that(skipped->name, is_equal_to_string("skipped"));
    assert_that(xmlChildElementCount(skipped), is_equal_to(0));
}


Ensure(LibXmlReporter, will_report_non_finishing_test) {
    const int line = 666;

    reporter->start_suite(reporter, "suite_name", 1);
    reporter->start_test(reporter, "test_name");
    send_reporter_exception_notification(reporter);
    reporter->finish_test(reporter, "filename", line, "message");
    reporter->finish_suite(reporter, "filename", line);

    xmlDocPtr doc = xmlParseDoc(output);
    assert_that(doc, is_not_null);

    xmlNodePtr testsuite = xmlDocGetRootElement(doc);
    assert_that(xmlChildElementCount(testsuite), is_equal_to(1));

    xmlNodePtr testcase = xmlFirstElementChild(testsuite);
    assert_that(testcase->name, is_equal_to_string("testcase"));
    assert_that(xmlChildElementCount(testcase), is_equal_to(1));
    assert_that(hasAttribute(testcase, XMLSTRING("time")), is_true);

    xmlNodePtr error = xmlFirstElementChild(testcase);
    assert_that(error->name, is_equal_to_string("error"));
    assert_that(error, xmlnode_has_attribute_equal_to("type", "Fatal"));
    assert_that(error, xmlnode_has_attribute_equal_to("message", "message"));
}

Ensure(LibXmlReporter, will_report_time_correctly_for_non_finishing_test) {
    const int line = 666;

    reporter->start_suite(reporter, "suite_name", 1);
    reporter->start_test(reporter, "test_name");
    send_reporter_exception_notification(reporter);
    reporter->finish_test(reporter, "filename", line, "message");
    reporter->finish_suite(reporter, "filename", line);

    assert_that(output, contains_string("name=\"test_name\""));
    assert_that(output, contains_string(" time=\""));
}

TestSuite *libxml_reporter_tests(void) {
    TestSuite *suite = create_test_suite();
    set_setup(suite, setup_xml_reporter_tests);

    add_test_with_context(suite, LibXmlReporter, will_report_beginning_of_suite);
    add_test_with_context(suite, LibXmlReporter, will_report_beginning_and_successful_finishing_of_passing_test);
    add_test_with_context(suite, LibXmlReporter, will_report_a_failing_test);
    add_test_with_context(suite, LibXmlReporter, will_report_a_failing_test_only_once);
    add_test_with_context(suite, LibXmlReporter, will_mark_ignored_test_as_skipped);
    add_test_with_context(suite, LibXmlReporter, will_report_non_finishing_test);
    add_test_with_context(suite, LibXmlReporter, will_report_time_correctly_for_non_finishing_test);

    set_teardown(suite, teardown_xml_reporter_tests);
    return suite;
}
