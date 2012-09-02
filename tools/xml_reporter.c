#include "xml_reporter.h"
#include "cgreen/reporter.h"
#include "cgreen/breadcrumb.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>


static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line);
static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void xml_show_incomplete(TestReporter *reporter, const char *filename, int line, const char *message, va_list arguments);

static const char *file_prefix;

TestReporter *create_xml_reporter(const char *prefix) {
    TestReporter *reporter = create_reporter();
    file_prefix = prefix;
    reporter->start_suite = &xml_reporter_start_suite;
    reporter->start_test = &xml_reporter_start_test;
    reporter->show_fail = &xml_show_fail;
    reporter->show_incomplete = &xml_show_incomplete;
    reporter->finish_test = &xml_reporter_finish_test;
    reporter->finish_suite = &xml_reporter_finish_suite;
    return reporter;
}

static int file_stack_p = 0;
static FILE *file_stack[100];

static void indent(FILE *out, TestReporter *reporter) {
    int depth = get_breadcrumb_depth((CgreenBreadcrumb *)reporter->breadcrumb);
    printf("depth = %d\n", depth); fflush(NULL);
    while (depth-- > 0) {
        fprintf(out, "\t");
    }
}

static void pathprinter(const char *trail, void *memo) {
    fprintf((FILE *)memo, "%s/", trail);
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *suitename, int count __attribute__((unused))) {
    char filename[80];
    sprintf(filename, "%s-%s.xml", file_prefix, suitename);
    FILE *out = fopen(filename, "w");
    file_stack[file_stack_p++] = out;
    fprintf(out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n");
    indent(out, reporter);
    fprintf(out, "<testsuite name=\"");
    walk_breadcrumb(reporter->breadcrumb, pathprinter, out);
    fprintf(out, "%s\">\n", suitename);
    reporter_start(reporter, suitename);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    FILE *out = file_stack[file_stack_p-1];
    indent(out, reporter);
    fprintf(out, "<testcase classname=\"");
    walk_breadcrumb(reporter->breadcrumb, pathprinter, out);
    fprintf(out, "\" name=\"%s\">\n", testname);
    reporter_start(reporter, testname);
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    FILE *out = file_stack[file_stack_p-1];
    indent(out, reporter);
    fprintf(out, "<failure message=\"");
    vfprintf(out, message, arguments);
    fprintf(out, "\">\n");
    indent(out, reporter);
    fprintf(out, "\t<location file=\"%s\" line=\"%d\"/>\n", file, line);
    indent(out, reporter);
    fprintf(out, "</failure>\n");
    fflush(out);
}

static void xml_show_incomplete(TestReporter *reporter, const char *filename __attribute__((unused)), int line __attribute__((unused)), const char *message, va_list arguments) {
    FILE *out = file_stack[file_stack_p-1];
    indent(out, reporter);
    fprintf(out, "<error type=\"Fatal\" message=\"");
    vfprintf(out, message, arguments);
    fprintf(out, "\">\n");
    indent(out, reporter);
    fprintf(out, "</error>\n");
    fflush(out);
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line) {
    FILE *out = file_stack[file_stack_p-1];
    reporter_finish(reporter, filename, line);
    indent(out, reporter);
    fprintf(out, "</testcase>\n");
    fflush(out);
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    FILE *out = file_stack[--file_stack_p];
    reporter_finish(reporter, filename, line);
    indent(out, reporter);
    fprintf(out, "</testsuite>\n");
    fclose(out);
}
