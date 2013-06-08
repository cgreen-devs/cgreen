#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "runner.h"
#include "cgreen/internal/runner_platform.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

typedef void (*sighandler_t)(int);

static int in_child_process();
static int wait_for_child_process();
static void stop();
static void ignore_ctrl_c(void);
static void allow_ctrl_c(void);


void run_test_in_its_own_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    (*reporter->start_test)(reporter, test->name);
    if (in_child_process()) {
        run_the_test_code(suite, test, reporter);
        send_reporter_completion_notification(reporter);
        stop();
    } else {
        const int status = wait_for_child_process();
        if (WIFSIGNALED(status)) {
            /* a C++ exception generates SIGABRT. Only print our special message for different signals. */
            const int sig = WTERMSIG(status);
            if (sig != SIGABRT) {
                char buf[128];
                snprintf(buf, sizeof(buf), "Test terminated with signal: %s", strsignal(sig));
                (*reporter->finish_test)(reporter, test->filename, test->line, buf);
                return;
            }
        }
        (*reporter->finish_test)(reporter, test->filename, test->line, NULL);
    }
}

static int in_child_process() {
    fflush(NULL);               /* Flush all buffers before forking */
    pid_t child = fork();
    if (child < 0) {
        die("Could not fork process\n");
    }
    return ! child;
}

static int wait_for_child_process() {
    int status;
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

static void stop() {
    _exit(EXIT_SUCCESS);
}

static void ignore_ctrl_c() {
    signal(SIGINT, SIG_IGN);
}

static void allow_ctrl_c() {
    signal(SIGINT, SIG_DFL);
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
