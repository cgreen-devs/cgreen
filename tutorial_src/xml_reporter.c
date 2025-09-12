#include "xml_reporter.h"
#include "cgreen/reporter.h"
#include "cgreen/breadcrumb.h"

#include <stdio.h>

static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line);
static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);

TestReporter *create_xml_reporter() {
    TestReporter *reporter = create_reporter();
    reporter->start_suite = &xml_reporter_start_suite;
    reporter->start_test = &xml_reporter_start_test;
    reporter->show_fail = &xml_show_fail;
    reporter->finish_test = &xml_reporter_finish_test;
    reporter->finish_suite = &xml_reporter_finish_suite;
    return reporter;
}

static indent(TestReporter *reporter) {
    int depth = get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb);
    while (depth-- > 0) {
        printf("\t");
    }
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count) {
    if (get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb) == 0) {
        printf("<?xml?>\n");
    }
    indent(reporter);
    printf("<suite name=\"%s\">\n", name);
    reporter_start(reporter, name);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *name) {
    indent(reporter);
    printf("<test name=\"%s\">\n", name);
    reporter_start(reporter, name);
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    indent(reporter);
    printf("<fail>\n");
    indent(reporter);
    printf("\t<message>");
    vprintf(message, arguments);
    printf("</message>\n");
    indent(reporter);
    printf("\t<location file=\"%s\" line=\"%d\"/>\n", file, line);
    indent(reporter);
    printf("</fail>\n");
}

static void xml_show_incomplete(TestReporter *reporter, const char *name) {
    indent(reporter);
    printf("<fail>\n");
    indent(reporter);
    printf("\t<message>Failed to complete]]></message>\n");
    indent(reporter);
    printf("</fail>\n");
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line) {
    reporter_finish_test(reporter, filename, line);
    indent(reporter);
    printf("</test>\n");
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    reporter_finish_suite(reporter, filename, line);
    indent(reporter);
    printf("</suite>\n");
}
