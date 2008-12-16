#include "xml_reporter.h"
#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdio.h>
#include <stdarg.h>

static indent(TestReporter *reporter);
static void xml_reporter_start(TestReporter *reporter, const char *name);
static void xml_reporter_finish(TestReporter *reporter, const char *name);
static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void xml_show_incomplete(TestReporter *reporter, const char *name);

TestReporter *create_xml_reporter() {
    TestReporter *reporter = create_reporter();
	reporter->start_suite = &xml_reporter_start;
	reporter->start_test = &xml_reporter_start;
	reporter->show_fail = &xml_show_fail;
	reporter->show_incomplete = &xml_show_incomplete;
	reporter->finish_test = &xml_reporter_finish;
	reporter->finish_suite = &xml_reporter_finish;
    return reporter;
}

static indent(TestReporter *reporter) {
    int depth = get_breadcrumb_depth((Breadcrumb *)reporter->breadcrumb);
    while (depth-- > 0) {
        printf("\t");
    }
}

static void xml_reporter_start(TestReporter *reporter, const char *name) {
    if (get_breadcrumb_depth((Breadcrumb *)reporter->breadcrumb) == 0) {
        printf("<?xml?>\n");
    }
	indent(reporter);
	printf("<test name=\"%s\">\n", name);
	reporter_start(reporter, name);
}

static void xml_reporter_finish(TestReporter *reporter, const char *name) {
	reporter_finish(reporter, name);
	indent(reporter);
	printf("</test>\n");
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
	indent(reporter);
    printf("<fail>\n");
	indent(reporter);
    printf("\t<message><![CDATA[");
    vprintf(message, arguments);
    printf("]]></message>\n");
	indent(reporter);
    printf("\t<location file=\"%s\" line=\"%d\"/>\n", file, line);
	indent(reporter);
    printf("</fail>\n");
}

static void xml_show_incomplete(TestReporter *reporter, const char *name) {
	indent(reporter);
    printf("<fail>\n");
	indent(reporter);
    printf("\t<message><![CDATA[Failed to complete]]></message>\n");
	indent(reporter);
    printf("</fail>\n");
}
