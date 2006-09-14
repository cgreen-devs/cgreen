#include "../cgreen.h"
#include "../sequence.h"
#include <stdlib.h>

int give_return_sequence() {
    return integer_sequence(1, 2, 3, 4, 5, 6);
}

void confirm_every_value_in_sequence() {
    assert_equal(give_return_sequence(), 1, "Expecting 1");
    assert_equal(give_return_sequence(), 2, "Expecting 2");
    assert_equal(give_return_sequence(), 3, "Expecting 3");
}

void show_that_the_sequence_restarts_across_tests() {
    assert_equal(give_return_sequence(), 1, "Expecting 1");
    assert_equal(give_return_sequence(), 2, "Expecting 2");
    assert_equal(give_return_sequence(), 3, "Expecting 3");
}

char *give_string_sequence() {
    return string_sequence("Hello", "Goodbye");
}

void confirm_every_string_in_sequence() {
    assert_string_equal(give_string_sequence(), "Hello", "Expecting Hello");
    assert_string_equal(give_string_sequence(), "Goodbye", "Expecting Goodbye");
}

void mock_callback(char *string, int integer, void *address) {
    assert_string_equal(string, string_sequence("Hello", "Goodbye"), "Got [%s]", string);
    assert_equal(integer, integer_sequence(1, 2), "Got [%d]", integer);
    assert_equal(address, address_sequence(NULL, (void *)1), "Got [%p]", address);
}

void feed_the_mock_callback_with_correct_arguments() {
    mock_callback("Hello", 1, NULL);
    mock_callback("Goodbye", 2, (void *)1);
}

void dont_call_me() {
    called_never();
}

void call_me_once() {
    called_once();
}

void call_me_twice() {
    expected_call_count(2);
}

void trip_the_counters() {
    call_me_once();
    call_me_twice();
    call_me_twice();
}

TestSuite *sequence_tests() {
    TestSuite *suite = create_test_suite();
    add_unit_test(suite, confirm_every_value_in_sequence);
    add_unit_test(suite, show_that_the_sequence_restarts_across_tests);
    add_unit_test(suite, confirm_every_string_in_sequence);
    add_unit_test(suite, feed_the_mock_callback_with_correct_arguments);
    add_unit_test(suite, trip_the_counters);
    return suite;
}
