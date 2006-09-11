#include "reporter.h"
#include "messaging.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <stdarg.h>

enum {pass = 1, fail, completion};

static void show_pass(TestReporter *reporter, const char *file, int line, char *message, va_list arguments);
static void show_fail(TestReporter *reporter, const char *file, int line, char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, char *name);
static void assert_true(TestReporter *reporter, const char *file, int line, int result, char *message, ...);
static void read_reporter_results(TestReporter *reporter);
static void create_breadcrumb(TestReporter *reporter);
static void destroy_breadcrumb(TestReporter *reporter);

TestReporter *create_reporter() {
    TestReporter *reporter = (TestReporter *)malloc(sizeof(TestReporter));
    reporter->destroy = &destroy_reporter;
	reporter->start = &reporter_start;
	reporter->finish = &reporter_finish;
	reporter->show_pass = &show_pass;
	reporter->show_fail = &show_fail;
	reporter->show_incomplete = &show_incomplete;
	reporter->assert_true = &assert_true;
	reporter->passes = 0;
	reporter->failures = 0;
	reporter->exceptions = 0;
	create_breadcrumb(reporter);
	reporter->ipc = start_ipc();
    return reporter;
}

void destroy_reporter(TestReporter *reporter) {
	destroy_breadcrumb(reporter);
    free(reporter);
}

void reporter_start(TestReporter *reporter, char *name)  {
	reporter->breadcrumb_depth++;
	if (reporter->breadcrumb_depth > reporter->breadcrumb_space) {
		reporter->breadcrumb_space++;
		reporter->breadcrumb = (char **)realloc(
				reporter->breadcrumb,
				sizeof(char *) * reporter->breadcrumb_space);
	}
	reporter->breadcrumb[reporter->breadcrumb_depth - 1] = name;
}

void reporter_finish(TestReporter *reporter, char *name) {
    read_reporter_results(reporter);
	reporter->breadcrumb_depth--;
}

void add_reporter_result(TestReporter *reporter, int result) {
    send_message((MessageQueue *)reporter->ipc, result ? pass : fail);
}

void send_reporter_completion_notification(TestReporter *reporter) {
    send_message((MessageQueue *)reporter->ipc, completion);
}

char *get_current_reporter_test(TestReporter *reporter) {
	return reporter->breadcrumb[reporter->breadcrumb_depth - 1];
}

void walk_reporter_breadcrumb(TestReporter *reporter, void (*walker)(char *, void *), void *memo) {
    int i;
    for (i = 0; i < reporter->breadcrumb_depth; i++) {
        (*walker)(reporter->breadcrumb[i], memo);
    }
}

static void show_pass(TestReporter *reporter, const char *file, int line, char *message, va_list arguments) {
}

static void show_fail(TestReporter *reporter, const char *file, int line, char *message, va_list arguments) {
}

static void show_incomplete(TestReporter *reporter, char *name) {
}

static void assert_true(TestReporter *reporter, const char *file, int line, int result, char *message, ...) {
    va_list arguments;
    va_start(arguments, message);
	if (result) {
    	(*reporter->show_pass)(reporter, file, line, message, arguments);
	} else {
    	(*reporter->show_fail)(reporter, file, line, message, arguments);
	}
	add_reporter_result(reporter, result);
	va_end(arguments);
}

static void read_reporter_results(TestReporter *reporter) {
    int completed = 0;
    int result;
    while ((result = receive_message((MessageQueue *)reporter->ipc)) > 0) {
        if (result == pass) {
            reporter->passes++;
        } else if (result == fail) {
            reporter->failures++;
        } else if (result == completion) {
            completed = 1;
        }
    }
    if (! completed) {
        (*reporter->show_incomplete)(reporter, get_current_reporter_test(reporter));
        reporter->exceptions++;
    }
}

static void create_breadcrumb(TestReporter *reporter) {
	reporter->breadcrumb = NULL;
	reporter->breadcrumb_depth = 0;
	reporter->breadcrumb_space = 0;
}

static void destroy_breadcrumb(TestReporter *reporter) {
	free(reporter->breadcrumb);
}
