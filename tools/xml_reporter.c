#include "xml_reporter.h"
#include "cgreen/reporter.h"
#include "cgreen/breadcrumb.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void xml_reporter_start_suite(TestReporter *reporter, const char *name, int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message,
                                     uint32_t duration_in_milliseconds);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line,
                                      uint32_t duration_in_milliseconds);
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

static void print_path_separator_if_needed(int *more_segments) {
    if (*more_segments > 0) {
        fprintf(file_stack[file_stack_p-1], "/");
        (*more_segments)--;
    }
}

static void fprint_path_segment(const char *segment, void *more_segments) {
    fprintf(file_stack[file_stack_p-1], "%s", segment);
    print_path_separator_if_needed((int*)more_segments);
}


#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
static char suite_path[PATH_MAX];

static void strcat_path_segment(const char *segment, void *more_segments) {
    (void)more_segments;
    if (suite_path[0] != '\0') strcat(suite_path, "-");
    strcat(suite_path, segment);
}

static void add_suite_name(const char *suite_name) {
    if (suite_path[0] != '\0')
        strcat(suite_path, "-");
    strcat(suite_path, suite_name);        
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *suitename, int count __attribute__((unused))) {
    char filename[PATH_MAX];
    int segment_decrementer = reporter->breadcrumb->depth;
    
    suite_path[0] = '\0';
    walk_breadcrumb(reporter->breadcrumb, strcat_path_segment, &segment_decrementer);
    add_suite_name(suitename);

    snprintf(filename, sizeof(filename), "%s-%s.xml", file_prefix, suite_path);
    FILE *out = fopen(filename, "w");
    if (!out) {
        fprintf(stderr, "could not open %s: %s\r\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    file_stack[file_stack_p++] = out;
    fprintf(out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n");
    indent(out, reporter);
    fprintf(out, "<testsuite name=\"%s\">\n", suite_path);
    reporter_start_suite(reporter, suitename, 0);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    FILE *out = file_stack[file_stack_p-1];
    indent(out, reporter);
    fprintf(out, "<testcase classname=\"");
    int segment_count = reporter->breadcrumb->depth - 1;
    walk_breadcrumb(reporter->breadcrumb, fprint_path_segment, &segment_count);

    // Don't terminate the XML-node now so that we can add the duration later
    fprintf(out, "\" name=\"%s\"", testname);
    reporter_start_test(reporter, testname);
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    FILE *out = file_stack[file_stack_p-1];
    fprintf(out, ">\n");
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
    fprintf(out, ">\n");
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

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message,
                                     uint32_t duration_in_milliseconds) {
    FILE *out = file_stack[file_stack_p-1];
    reporter_finish_test(reporter, filename, line, message, duration_in_milliseconds);
    fprintf(out, " time=\"%.5f\">\n", (double)duration_in_milliseconds/(double)1000);
    indent(out, reporter);
    fprintf(out, "</testcase>\n");
    fflush(out);
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line,
                                      uint32_t duration_in_milliseconds) {
    FILE *out = file_stack[--file_stack_p];
    reporter_finish_suite(reporter, filename, line, duration_in_milliseconds);

    // TODO: Here we should backpatch the time for the suite but that's not
    // exactly necessary as Jenkins, at least, seems to sum it up automatically
    indent(out, reporter);
    fprintf(out, "</testsuite>\n");
    fclose(out);
}
