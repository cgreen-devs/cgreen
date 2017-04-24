#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef __ANDROID__
#include "config.h"
#endif // #ifdef __ANDROID__
#include "runner.h"
#include "cgreen/internal/runner_platform.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <cgreen/internal/cgreen_time.h>

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__


typedef void (*sighandler_t)(int);

static int in_child_process(void);
static int wait_for_child_process(void);
static void stop(void);
static void ignore_ctrl_c(void);
static void allow_ctrl_c(void);


void run_test_in_its_own_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    uint32_t test_starting_milliseconds = cgreen_time_get_current_milliseconds();

    (*reporter->start_test)(reporter, test->name);
    if (test->skip) {
        send_reporter_skipped_notification(reporter);
        (*reporter->finish_test)(reporter, test->filename, test->line, NULL);
    } else if (in_child_process()) {
        run_the_test_code(suite, test, reporter);
        send_reporter_completion_notification(reporter);
        stop();
    } else {
        const int status = wait_for_child_process();
        reporter->duration = cgreen_time_duration_in_milliseconds(test_starting_milliseconds,
                                                                      cgreen_time_get_current_milliseconds());
        if (WIFSIGNALED(status)) {
            /* a C++ exception generates SIGABRT. Only print our special message for other signals. */
            const int sig = WTERMSIG(status);
            if (sig != SIGABRT) {
                char buf[128];
                snprintf(buf, sizeof(buf), "Test terminated with signal: %s", (const char *)strsignal(sig));
                (*reporter->finish_test)(reporter, test->filename, test->line, buf);
                return;
            }
        }

        (*reporter->finish_test)(reporter, test->filename, test->line, NULL);
    }
}

static int in_child_process(void) {
    fflush(NULL);               /* Flush all buffers before forking */
    pid_t child = fork();
    if (child < 0) {
        die("Could not fork process\n");
    }
    return ! child;
}

static int wait_for_child_process(void) {
    int status = 0;
    ignore_ctrl_c();
    wait(&status);
    allow_ctrl_c();
    return status;
}

void die_in(unsigned int seconds) {
    sighandler_t signal_result = signal(SIGALRM, (sighandler_t)&stop);
    if (SIG_ERR == signal_result) {
        fprintf(stderr, "could not set alarm signal hander\n");
        return;
    }

    alarm(seconds);
}

void run_specified_test_if_child(TestSuite *suite, TestReporter *reporter){
    //not needed for systems that support fork()
    (void)suite;
    (void)reporter;
}

static void stop(void) {
#ifdef CGREEN_INTERNAL_WITH_GCOV
    if (1)
#else
    if (getenv("CGREEN_CHILD_EXIT_WITH__EXIT") == NULL)
#endif
        exit(EXIT_SUCCESS);
    else
        _exit(EXIT_SUCCESS);
}

static void ignore_ctrl_c(void) {
    signal(SIGINT, SIG_IGN);
}

static void allow_ctrl_c(void) {
    signal(SIGINT, SIG_DFL);
}

/* vim: set ts=4 sw=4 et cindent: */
