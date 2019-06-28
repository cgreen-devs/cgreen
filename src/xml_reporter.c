#include <cgreen/breadcrumb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include "xml_reporter_internal.h"

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

typedef struct {
    XmlPrinter *printer;
    int segment_count;
} XmlMemo;


static void xml_reporter_start_suite(TestReporter *reporter, const char *name,
                                     int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename,
                                     int line, const char *message);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename,
                                      int line);
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

static char *indent(TestReporter *reporter) {
    static char buffer[1000];
    int depth = get_breadcrumb_depth(reporter->breadcrumb);

    memset(buffer, '\0', depth+1);
    memset(buffer, '\t', depth);
    return buffer;
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
    FILE *out;

    (void)count;                /* UNUSED */

    reporter->passes = 0;
    reporter->failures = 0;
    reporter->skips = 0;
    reporter->exceptions = 0;

    suite_path[0] = '\0';
    walk_breadcrumb(reporter->breadcrumb, strcat_path_segment, &segment_decrementer);
    add_suite_name(suitename);

    snprintf(filename, sizeof(filename), "%s-%s.xml", file_prefix, suite_path);
    if (memo->printer == fprintf) {
        // If we're really printing to files, then open one...
        out = fopen(filename, "w");
        if (!out) {
            memo->printer(stderr, "could not open %s: %s\r\n", filename, strerror(errno));
            exit(EXIT_FAILURE);
        }
    } else
        out = stdout;

    file_stack[file_stack_p++] = out;
    memo->printer(out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n");
    memo->printer(out, indent(reporter));
    memo->printer(out, "<testsuite name=\"%s\">\n", suite_path);
    reporter_start_suite(reporter, suitename, 0);
}


/* Accumulate output from the actual test (the "<testcase>" nodes) in
   a file since the tests usually are run in a child processes, so
   there is no simple way to save output from it and then use it in
   the parent (start_test() and finish_test() are run from parent) */

static char *output = NULL;
static FILE *child_output_tmpfile;

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    memo->printer(out, indent(reporter));
    memo->printer(out, "<testcase classname=\"");
    memo->segment_count = reporter->breadcrumb->depth - 1;
    walk_breadcrumb(reporter->breadcrumb, print_path_segment_walker, memo);

    // Don't terminate the XML-node now so that we can add the duration later
    // But then we need to accumulate subsequent output to report later
    memo->printer(out, "\" name=\"%s\"", testname);
    reporter_start_test(reporter, testname);
    output = strdup("");

    child_output_tmpfile = tmpfile();
}


static char *concat(char *head, const char *tail) {
    char *result = realloc(head, strlen(head)+strlen(tail)+1);
    return strcat(result, tail);
}

static void xml_show_skip(TestReporter *reporter, const char *file, int line) {
    (void)file;
    (void)line;

    output = concat(output, indent(reporter));
    output = concat(output, "\t<skipped />\n");

    fseek(child_output_tmpfile,0,SEEK_SET);
    fputs(output, child_output_tmpfile);
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    char buffer[1000];

    output = concat(output, indent(reporter));
    output = concat(output, "<failure message=\"");

    vsnprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), message, arguments);
    output = concat(output, buffer);
    output = concat(output, "\">\n");
    output = concat(output, indent(reporter));

    snprintf(buffer, sizeof(buffer)/sizeof(buffer[0]),
             "\t<location file=\"%s\" line=\"%d\"/>\n", file, line);
    output = concat(output, buffer);
    output = concat(output, indent(reporter));
    output = concat(output, "</failure>\n");

    fseek(child_output_tmpfile,0,SEEK_SET);
    fputs(output, child_output_tmpfile);
}

static void xml_show_incomplete(TestReporter *reporter, const char *filename, int line, const char *message, va_list arguments) {
    char buffer[1000];

    output = concat(output, indent(reporter));
    output = concat(output, "<error type=\"Fatal\" message=\"");
    vsnprintf(buffer, sizeof(buffer)/sizeof(buffer[0]),
    		message ? message: "Test terminated unexpectedly, likely from a non-standard exception or Posix signal", arguments);
    output = concat(output, buffer);
    output = concat(output, "\">\n");
    output = concat(output, indent(reporter));
    snprintf(buffer, sizeof(buffer)/sizeof(buffer[0]),"\t<location file=\"%s\" line=\"%d\"/>\n", filename, line);
    output = concat(output, buffer);
    output = concat(output, indent(reporter));
    output = concat(output, "</error>\n");

    fseek(child_output_tmpfile,0,SEEK_SET);
    fputs(output, child_output_tmpfile);
}


static void transfer_output_from(FILE *tmpfile, XmlPrinter printer, FILE *out) {
    fseek(tmpfile, 0, SEEK_SET);
    char buffer[1000];
    while (fgets(buffer, 1000, tmpfile) != NULL)
        printer(out, buffer);
}


static void xml_reporter_finish_test(TestReporter *reporter, const char *filename, int line, const char *message) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[file_stack_p-1];

    reporter_finish_test(reporter, filename, line, message);
    memo->printer(out, " time=\"%.5f\">\n", (double)reporter->duration/(double)1000);
    if (output && strlen(output) == 0) {
        free(output);
        output = NULL;
    }

    transfer_output_from(child_output_tmpfile, memo->printer, out);

    memo->printer(out, indent(reporter));
    memo->printer(out, "</testcase>\n");
    fflush(out);
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    FILE *out = file_stack[--file_stack_p];

    reporter_finish_suite(reporter, filename, line);

    reporter->total_passes += reporter->passes;
    reporter->total_failures += reporter->failures;
    reporter->total_skips += reporter->skips;
    reporter->total_exceptions += reporter->exceptions;

    // TODO: Here we should backpatch the time for the suite but that's not
    // exactly necessary as Jenkins, at least, seems to sum it up automatically
    memo->printer(out, indent(reporter));
    memo->printer(out, "</testsuite>\n");
    if (file_stack_p != 0)
        fclose(out);
}
