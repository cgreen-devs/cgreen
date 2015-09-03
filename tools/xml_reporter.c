#include "xml_reporter.h"
#include "cgreen/reporter.h"
#include "cgreen/breadcrumb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message);
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
    int depth = get_breadcrumb_depth(reporter->breadcrumb);
    while (depth-- > 0) {
        fprintf(out, "\t");
    }
}

static void print_separator_if_needed(int *more_segments) {
    if (*more_segments > 0) {
        fprintf(file_stack[file_stack_p-1], "/");
        (*more_segments)--;
    }
}

static void pathprinter(const char *segment, void *more_segments) {
    fprintf(file_stack[file_stack_p-1], "%s", segment);
	print_separator_if_needed((int*)more_segments);
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *suitename, int count __attribute__((unused))) {
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
    char filename[PATH_MAX];
    snprintf(filename, sizeof(filename), "%s-%s.xml", file_prefix, suitename);
    FILE *out = fopen(filename, "w");
    if (!out) {
        fprintf(stderr, "could not open %s: %s\r\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    file_stack[file_stack_p++] = out;
    fprintf(out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n");
    indent(out, reporter);
    fprintf(out, "<testsuite name=\"");
    int segment_count = reporter->breadcrumb->depth;
    walk_breadcrumb(reporter->breadcrumb, pathprinter, &segment_count);
    fprintf(out, "%s\">\n", suitename);
    reporter_start(reporter, suitename);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    FILE *out = file_stack[file_stack_p-1];
    indent(out, reporter);
    fprintf(out, "<testcase classname=\"");
    int segment_count = reporter->breadcrumb->depth - 1;
    walk_breadcrumb(reporter->breadcrumb, pathprinter, &segment_count);
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

static void xml_show_incomplete(TestReporter *reporter, const char *filename, int line, const char *message, va_list arguments) {
    FILE *out = file_stack[file_stack_p-1];
    indent(out, reporter);
    fprintf(out, "<error type=\"Fatal\" message=\"");
    vfprintf(out, message ? message: "Test terminated unexpectedly, likely from a non-standard exception or Posix signal", arguments);
    fprintf(out, "\">\n");
    indent(out, reporter);
    fprintf(out, "\t<location file=\"%s\" line=\"%d\"/>\n", filename, line);
    indent(out, reporter);
    fprintf(out, "</error>\n");
    fflush(out);
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message) {
    FILE *out = file_stack[file_stack_p-1];
    reporter_finish(reporter, filename, line, message);
    indent(out, reporter);
    fprintf(out, "</testcase>\n");
    fflush(out);
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    FILE *out = file_stack[--file_stack_p];
    reporter_finish_suite(reporter, filename, line);
    indent(out, reporter);
    fprintf(out, "</testsuite>\n");
    fclose(out);
}
