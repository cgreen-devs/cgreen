#include <cgreen/breadcrumb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include "xml_reporter_internal.h"


typedef struct {
    XmlPrinter *printer;
    int segment_count;
} XmlMemo;


static void xml_reporter_start_suite(TestReporter *reporter, const char *name,
                                     int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename,
                                     int line, const char *message,
                                     uint32_t duration_in_milliseconds);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename,
                                      int line, uint32_t duration_in_milliseconds);
static void xml_show_skip(TestReporter *reporter, const char *file, int line);
static void xml_show_fail(TestReporter *reporter, const char *file, int line,
                          const char *message, va_list arguments);
static void xml_show_incomplete(TestReporter *reporter, const char *filename,
                                int line, const char *message, va_list arguments);


void set_xml_reporter_printer(TestReporter *reporter, XmlPrinter *new_printer) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    memo->printer = new_printer;
}

static const char *file_prefix;

TestReporter *create_xml_reporter(const char *prefix) {
    TestReporter *reporter;
    XmlMemo *memo;
    
    reporter = create_reporter();
    if (reporter == NULL) {
        return NULL;
    }

    memo = (XmlMemo *) malloc(sizeof(XmlMemo));
    if (memo == NULL) {
        destroy_reporter(reporter);
        return NULL;
    }
    memo->printer = fprintf;
    reporter->memo = memo;
    
    file_prefix = prefix;
    reporter->start_suite = &xml_reporter_start_suite;
    reporter->start_test = &xml_reporter_start_test;
    reporter->show_fail = &xml_show_fail;
    reporter->show_skip = &xml_show_skip;
    reporter->show_incomplete = &xml_show_incomplete;
    reporter->finish_test = &xml_reporter_finish_test;
    reporter->finish_suite = &xml_reporter_finish_suite;
    return reporter;
}

static int file_stack_p = 0;
static FILE *file_stack[100];

static void indent(FILE *out, TestReporter *reporter) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    int depth = get_breadcrumb_depth(reporter->breadcrumb);
    while (depth-- > 0) {
        memo->printer(out, "\t");
    }
}

static void print_path_separator_if_needed(XmlMemo *memo, int *more_segments) {
    if (*more_segments > 0) {
        memo->printer(file_stack[file_stack_p-1], "/");
        (*more_segments)--;
    }
}

static void print_path_segment_walker(const char *segment, void *void_memo) {
    XmlMemo *memo = (XmlMemo *)void_memo;

    memo->printer(file_stack[file_stack_p-1], "%s", segment);
    print_path_separator_if_needed(memo, &memo->segment_count);
}


#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
static char suite_path[PATH_MAX];

static void strcat_path_segment(const char *segment, void *more_segments) {
    (void)more_segments;
    if (suite_path[0] != '\0') strcat(suite_path, "-");
    strncat(suite_path, segment, sizeof(suite_path)-strlen(suite_path)-1);
}

static void add_suite_name(const char *suite_name) {
    if (suite_path[0] != '\0')
        strcat(suite_path, "-");
    strncat(suite_path, suite_name, sizeof(suite_path)-strlen(suite_path)-1);
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *suitename, int count) {
    char filename[PATH_MAX];
    int segment_decrementer = reporter->breadcrumb->depth;
    XmlMemo *memo = (XmlMemo *)reporter->memo;

    (void)count;                /* UNUSED */
    
    suite_path[0] = '\0';
    walk_breadcrumb(reporter->breadcrumb, strcat_path_segment, &segment_decrementer);
    add_suite_name(suitename);

    snprintf(filename, sizeof(filename), "%s-%s.xml", file_prefix, suite_path);
    FILE *out = fopen(filename, "w");
    if (!out) {
        memo->printer(stderr, "could not open %s: %s\r\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    file_stack[file_stack_p++] = out;
    memo->printer(out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n");
    indent(out, reporter);
    memo->printer(out, "<testsuite name=\"%s\">\n", suite_path);
    reporter_start_suite(reporter, suitename, 0);
}

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    indent(out, reporter);
    memo->printer(out, "<testcase classname=\"");
    memo->segment_count = reporter->breadcrumb->depth - 1;
    walk_breadcrumb(reporter->breadcrumb, print_path_segment_walker, memo);

    // Don't terminate the XML-node now so that we can add the duration later
    memo->printer(out, "\" name=\"%s\"", testname);
    reporter_start_test(reporter, testname);
}

static void xml_show_skip(TestReporter *reporter, const char *file, int line) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    (void)file;
    (void)line;
    memo->printer(out, ">\n");
    indent(out, reporter);
    memo->printer(out, "<skipped />");
    fflush(out);
}
static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    memo->printer(out, ">\n");
    indent(out, reporter);
    memo->printer(out, "<failure message=\"");
    memo->printer(out, message, arguments);
    memo->printer(out, "\">\n");
    indent(out, reporter);
    memo->printer(out, "\t<location file=\"%s\" line=\"%d\"/>\n", file, line);
    indent(out, reporter);
    memo->printer(out, "</failure>\n");
    fflush(out);
}

static void xml_show_incomplete(TestReporter *reporter, const char *filename, int line, const char *message, va_list arguments) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    memo->printer(out, ">\n");
    indent(out, reporter);
    memo->printer(out, "<error type=\"Fatal\" message=\"");
    memo->printer(out, message ? message: "Test terminated unexpectedly, likely from a non-standard exception or Posix signal", arguments);
    memo->printer(out, "\">\n");
    indent(out, reporter);
    memo->printer(out, "\t<location file=\"%s\" line=\"%d\"/>\n", filename, line);
    indent(out, reporter);
    memo->printer(out, "</error>\n");
    fflush(out);
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message,
                                     uint32_t duration_in_milliseconds) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    reporter_finish_test(reporter, filename, line, message, duration_in_milliseconds);
    memo->printer(out, " time=\"%.5f\">\n", (double)duration_in_milliseconds/(double)1000);
    indent(out, reporter);
    memo->printer(out, "</testcase>\n");
    fflush(out);
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line,
                                      uint32_t duration_in_milliseconds) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[--file_stack_p];

    reporter_finish_suite(reporter, filename, line, duration_in_milliseconds);

    // TODO: Here we should backpatch the time for the suite but that's not
    // exactly necessary as Jenkins, at least, seems to sum it up automatically
    indent(out, reporter);
    memo->printer(out, "</testsuite>\n");
    fclose(out);
}
