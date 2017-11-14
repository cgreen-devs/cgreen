#include <cgreen/cgreen.h>
#include <cgreen/messaging.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/utils.h"

#include <signal.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Ensure(highly_nested_test_suite_should_still_complete) {
    assert_that(true);
}

TestSuite *highly_nested_test_suite(void) {
    int i;
    TestSuite *suite = create_test_suite();
    add_test(suite, highly_nested_test_suite_should_still_complete);
    for (i = 0; i < 1000; i++) {
        TestSuite *nesting = create_test_suite();
        add_suite(nesting, suite);
        suite = nesting;
    }
    return suite;
}

Ensure(can_send_message) {
    int messaging = start_cgreen_messaging(33);
    send_cgreen_message(messaging, 99);
    assert_that(receive_cgreen_message(messaging), is_equal_to(99));
}

static int signal_received = 0;
static void catch_signal(int s) {
    (void)s;
    signal_received = 1;
    signal(SIGPIPE, SIG_DFL);
}

Ensure(failure_reported_and_exception_thrown_when_messaging_would_block) {
    const int LOOPS = 65536;
    int messaging = start_cgreen_messaging(33);
    int loop;
    char panic_message[1000];

    signal_received = 0;
    signal(SIGPIPE, catch_signal);

    panic_set_output_buffer(panic_message);

    for (loop = 0; loop < LOOPS; loop++) {
        send_cgreen_message(messaging, 99);
        if (signal_received == 1)
            break;
    }

    assert_that(signal_received, is_equal_to(1));
    assert_that(loop, is_less_than(LOOPS));
    assert_that(panic_message, contains_string("Too many assertions"));
}

TestSuite *messaging_tests(void) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, highly_nested_test_suite());
    add_test(suite, can_send_message);
#ifndef WIN32 // TODO: win32 needs non-blocking pipes like posix for this to pass
    add_test(suite, failure_reported_and_exception_thrown_when_messaging_would_block);
#endif
    return suite;
}
