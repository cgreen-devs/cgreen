#include "../cgreen.h"
#include "../messaging.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>

Ensure highly_nested_test_suite_should_still_complete() {
    assert_true(1);
}

TestSuite *highly_nested_test_suite() {
    TestSuite *suite = create_test_suite();
    add_test(suite, highly_nested_test_suite_should_still_complete);
    int i;
    for (i = 0; i < 1000; i++) {
        TestSuite *nesting = create_test_suite();
        add_suite(nesting, suite);
        suite = nesting;
    }
    return suite;
}

Ensure can_send_message() {
    int messaging = start_cgreen_messaging(33);
    send_cgreen_message(messaging, 99);
    assert_equal(receive_cgreen_message(messaging), 99);
}

TestSuite *messaging_tests() {
    TestSuite *suite = create_test_suite();
    add_suite(suite, highly_nested_test_suite());
    add_test(suite, can_send_message);
    return suite;
}
