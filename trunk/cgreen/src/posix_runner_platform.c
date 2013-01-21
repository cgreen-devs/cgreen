#include "runner.h"
#include "cgreen/runner_platform.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>


#ifdef __cplusplus
namespace cgreen {
#endif

typedef void (*sighandler_t)(int);

static int in_child_process();
static void wait_for_child_process();
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
        wait_for_child_process();
        (*reporter->finish_test)(reporter, test->filename, test->line);
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

static void wait_for_child_process() {
    int status;
    ignore_ctrl_c();
    wait(&status);
    allow_ctrl_c();
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
