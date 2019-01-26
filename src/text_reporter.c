#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <cgreen/text_reporter.h>
#include "text_reporter_internal.h"

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"


static void text_reporter_start_suite(TestReporter *reporter, const char *name,
        const int number_of_tests);
static void text_reporter_start_test(TestReporter *reporter, const char *name);
static void text_reporter_finish(TestReporter *reporter, const char *filename,
        int line, const char *message);
static void show_fail(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments);
static void show_breadcrumb(const char *name, void *memo);
static void text_reporter_finish_suite(TestReporter *reporter, const char *file, int line);


/* To be able to run a reporter as CUT for testing with Cgreen itself
   we need two reporters simultaneously, so the injected printer needs
   to be local to the reporter which means we must store it in the
   memo and use that. This requires a single printer function to be
   used, often printf() or similar, so any other tricks needs to be
   performed in char buffers so that memo->printer can do the
   printing.
 */
typedef struct {
    TextPrinter *printer;
    TextVPrinter *vprinter;
    int depth;
} TextMemo;


void set_text_reporter_printer(TestReporter *reporter, TextPrinter *new_printer) {
    TextMemo *memo = (TextMemo *)reporter->memo;
    memo->printer = new_printer;
}

void set_text_reporter_vprinter(TestReporter *reporter, TextVPrinter *new_vprinter) {
    TextMemo *memo = (TextMemo *)reporter->memo;
    memo->vprinter = new_vprinter;
}

TestReporter *create_text_reporter(void) {
    TextMemo *memo;
    TestReporter *reporter = create_reporter();

    if (reporter == NULL) {
        return NULL;
    }

    memo = (TextMemo *)malloc(sizeof(TextMemo));
    if (memo == NULL) {
        destroy_reporter(reporter);
        return NULL;
    }
    reporter->memo = memo;

    reporter->start_suite = &text_reporter_start_suite;
    reporter->start_test = &text_reporter_start_test;
    reporter->show_fail = &show_fail;
    reporter->show_incomplete = &show_incomplete;
    reporter->finish_test = &text_reporter_finish;
    reporter->finish_suite = &text_reporter_finish_suite;

    set_text_reporter_printer(reporter, printf);
    set_text_reporter_vprinter(reporter, vprintf);

    return reporter;
}


static bool have_quiet_mode(TestReporter *reporter) {
    return reporter->options&&((TextReporterOptions *)reporter->options)->quiet_mode;
}

static bool inhibit_start_suite_message(TestReporter *reporter) {
    return reporter->options&&((TextReporterOptions *)reporter->options)->inhibit_start_suite_message;
}

static bool inhibit_finish_suite_message(TestReporter *reporter) {
    return reporter->options&&((TextReporterOptions *)reporter->options)->inhibit_finish_suite_message;
}

static void text_reporter_start_suite(TestReporter *reporter, const char *name,
        const int number_of_tests) {
    TextMemo *memo = (TextMemo *)reporter->memo;

    reporter->passes = 0;
    reporter->failures = 0;
    reporter->skips = 0;
    reporter->exceptions = 0;

    reporter_start_test(reporter, name);
    if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 1) {
        if (!have_quiet_mode(reporter) && !inhibit_start_suite_message(reporter))
            memo->printer("Running \"%s\" (%d test%s)...\n",
                          get_current_from_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb),
                          number_of_tests,
                          number_of_tests==1?"":"s");
        fflush(stdout);
    }
}

static void text_reporter_start_test(TestReporter *reporter, const char *name) {
    reporter_start_test(reporter, name);
}

static void text_reporter_finish(TestReporter *reporter, const char *filename,
        int line, const char *message) {
    reporter_finish_test(reporter, filename, line, message);
}


static void format_count(char *buff, int count, const char *name, const char *color, const char *plural, bool use_colors) {
    const char *color_on = use_colors?color:"";
    const char *color_off = use_colors?RESET:"";
    sprintf(buff, "%s%d %s%s%s", count>0?color_on:"", count, name, count==1?"":plural, color_off);
}

static char *format_passes(int passes, bool use_colors) {
    static char buff[100];
    format_count(buff, passes, "pass", GREEN, "es", use_colors);
    return buff;
}

static char *format_skips(int skips, bool use_colors) {
    static char buff[100];
    format_count(buff, skips, "skipped", YELLOW, "", use_colors);
    return buff;
}

static char *format_failures(int failures, bool use_colors) {
    static char buff[100];
    format_count(buff, failures, "failure", RED, "s", use_colors);
    return buff;
}

static char *format_exceptions(int exceptions, bool use_colors) {
    static char buff[100];
    format_count(buff, exceptions, "exception", MAGENTA, "s", use_colors);
    return buff;
}

static char *format_duration(uint32_t duration) {
    static char buff[100];
    snprintf(buff, sizeof(buff), " in %dms", duration);
    return buff;
}

static void insert_comma(char buf[]) {
    if (buf[strlen(buf)-1] != ' ')
        strcat(buf, ", ");
}

static void text_reporter_print_results(char *buf, char *prepend,
    int passes, int failures, int skips, int exceptions, uint32_t duration,
    bool use_colors) {

    sprintf(buf, "%s", prepend);
    if (passes || failures || skips || exceptions) {
        if (passes)
            strcat(buf, format_passes(passes, use_colors));
        if (skips) {
            insert_comma(buf);
            strcat(buf, format_skips(skips, use_colors));
        }
        if (failures) {
            insert_comma(buf);
            strcat(buf, format_failures(failures, use_colors));
        }
        if (exceptions) {
            insert_comma(buf);
            strcat(buf, format_exceptions(exceptions, use_colors));
        }
        strcat(buf, format_duration(duration));
    } else {
        strcat(buf, "No asserts");
    }
}

static void text_reporter_finish_suite(TestReporter *reporter, const char *file, int line) {
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb);
    bool use_colors = reporter->options && ((TextReporterOptions *)reporter->options)->use_colours;
    TextMemo *memo = (TextMemo *)reporter->memo;

    reporter_finish_suite(reporter, file, line);

    reporter->total_passes += reporter->passes;
    reporter->total_failures += reporter->failures;
    reporter->total_skips += reporter->skips;
    reporter->total_exceptions += reporter->exceptions;

    if (have_quiet_mode(reporter)) {
        if (use_colors) {
            memo->printer(GREEN);
            if (reporter->failures) memo->printer(RED);
            if (reporter->exceptions) memo->printer(MAGENTA);
        }
        if (reporter->exceptions) memo->printer("X");
        else if (reporter->failures) memo->printer("F");
        else memo->printer(".");
        if (use_colors) {
            memo->printer(RESET);
        }
    } else {
        char buf[1000];
        char prepend[100];

        sprintf(prepend, "  \"%s\": ", name);
        text_reporter_print_results(buf, prepend,
                reporter->passes,
                reporter->failures,
                reporter->skips,
                reporter->exceptions,
                reporter->duration,
                use_colors);

        // Don't report top-level (pseudo-suite) if it had no asserts at all
        if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) != 0 ||
                (reporter->passes || reporter->failures || reporter->skips || reporter->exceptions)) {
            memo->printer("%s.\n", buf);
        }

        // Report totals
        if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 0) {
            if (!have_quiet_mode(reporter) && !inhibit_finish_suite_message(reporter)) {
                sprintf(prepend, "Completed \"%s\": ", name);
                text_reporter_print_results(buf, prepend,
                                            reporter->total_passes,
                                            reporter->total_failures,
                                            reporter->total_skips,
                                            reporter->total_exceptions,
                                            reporter->total_duration,
                                            use_colors);
                memo->printer("%s.\n", buf);
            }
        }
    }
}

static void show_fail(TestReporter *reporter, const char *file, int line,
                      const char *message, va_list arguments) {
    TextMemo *memo = (TextMemo *)reporter->memo;
    if (have_quiet_mode(reporter)) memo->printer("\n");
    memo->printer("%s:%d: ", file, line);
    memo->printer("Failure: ");
    memo->depth = 0;
    walk_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, &show_breadcrumb, memo);
    memo->printer("\n\t");
    // Simplify *printf statements for more robust cross-platform logging
    if (message == NULL) {
        memo->printer("<FATAL: NULL for failure message>");
    } else {
        memo->vprinter(message, arguments);
    }
    memo->printer("\n");
    memo->printer("\n");
    fflush(NULL);
}

static void show_incomplete(TestReporter *reporter, const char *file, int line,
                            const char *message, va_list arguments) {
    TextMemo *memo = (TextMemo *)reporter->memo;

    memo->printer("%s:%d: ", file, line);
    memo->printer("Exception: ");

    memo->depth = 0;
    walk_breadcrumb((CgreenBreadcrumb *) reporter->breadcrumb, &show_breadcrumb,
                    memo);

    memo->printer("\n\t");
    if (message == NULL) {
        memo->printer("Test terminated unexpectedly, "
                "likely from a non-standard exception or Posix signal");
    } else {
        memo->vprinter(message, arguments);
    }
    memo->printer("\n");
    memo->printer("\n");
    fflush(NULL);
}

static void show_breadcrumb(const char *name, void *memo_ptr) {
    TextMemo *memo = (TextMemo *)memo_ptr;
    if (memo->depth > 1) {
        memo->printer("-> ");
    }
    if (memo->depth > 0) {
        memo->printer("%s ", name);
    }
    memo->depth++;
}

/* vim: set ts=4 sw=4 et cindent: */
/* Local variables: */
/* tab-width: 4     */
/* End:             */
