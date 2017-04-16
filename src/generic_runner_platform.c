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
#include <stdlib.h>
#include <string.h>
#include <cgreen/internal/cgreen_time.h>

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

// FIXME: check for GCC

void run_test_in_its_own_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    uint32_t test_starting_milliseconds = cgreen_time_get_current_milliseconds();

    (*reporter->start_test)(reporter, test->name);
    if (test->skip) {
        send_reporter_skipped_notification(reporter);
        (*reporter->finish_test)(reporter, test->filename, test->line, NULL, 0);
    } else {
        run_the_test_code(suite, test, reporter);
        send_reporter_completion_notification(reporter);
        uint32_t test_duration = cgreen_time_duration_in_milliseconds(test_starting_milliseconds,
                                                                      cgreen_time_get_current_milliseconds());

        (*reporter->finish_test)(reporter, test->filename, test->line, NULL, test_duration);
    }
}

void run_specified_test_if_child(TestSuite *suite, TestReporter *reporter){
    //not needed for systems that support fork()
    (void)suite;
    (void)reporter;
}

uint32_t cgreen_time_get_current_milliseconds() {
    // FIXME: fake implementation
    static ms=0;
    return ++ms;
}

void die_in(unsigned int seconds) {
    // FIXME: fake implementation
    (void)seconds;
}

/* vim: set ts=4 sw=4 et cindent: */
