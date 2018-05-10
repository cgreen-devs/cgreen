#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>

#include <stdio.h>
#include "xml_reporter.h"


static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count) {
    printf("<suite name=\"%s\">\n", name);
    reporter_start_suite(reporter, name, count);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *name) {
    printf("<test name=\"%s\">\n", name);
    reporter_start_test(reporter, name);
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message) {
    reporter_finish_test(reporter, filename, line, message);
    printf("</test>\n");
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    reporter_finish_suite(reporter, filename, line);
    printf("</suite>\n");
}

TestReporter *create_xml_reporter() {
    TestReporter *reporter = create_reporter();
    reporter->start_suite = &xml_reporter_start_suite;
    reporter->start_test = &xml_reporter_start_test;
    reporter->finish_test = &xml_reporter_finish_test;
    reporter->finish_suite = &xml_reporter_finish_suite;
    return reporter;
}
