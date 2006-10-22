#include "../cgreen.h"
#include "../mocks.h"
#include <stdlib.h>

static int integer_out() {
    return (int)stubbed_result();
}

static void can_stub_an_integer_return() {
    will_return(integer_out, 3);
    assert_equal(integer_out(), 3);
}

static void repeats_return_value_when_set_to_always() {
    always_return(integer_out, 3);
    assert_equal(integer_out(), 3);
    assert_equal(integer_out(), 3);
}

static void can_stub_an_integer_return_sequence() {
    will_return(integer_out, 1);
    will_return(integer_out, 2);
    will_return(integer_out, 3);
    assert_equal(integer_out(), 1);
    assert_equal(integer_out(), 2);
    assert_equal(integer_out(), 3);
}

static void set_stub_just_to_be_cleared() {
    will_return(integer_out, 1);
}

static void confirm_stub_is_reset_between_tests() {
    will_return(integer_out, 2);
    assert_equal(integer_out(), 2);
}

static void stub_uses_the_always_value_once_hit() {
    will_return(integer_out, 1);
    will_return(integer_out, 2);
    always_return(integer_out, 3);
    will_return(integer_out, 4);
    assert_equal(integer_out(), 1);
    assert_equal(integer_out(), 2);
    assert_equal(integer_out(), 3);
    assert_equal(integer_out(), 3);
    assert_equal(integer_out(), 3);
}

static char *string_out() {
    return (char *)stubbed_result();
}

static void can_stub_a_string_return() {
    will_return(string_out, "hello");
    assert_equal(string_out(), "hello");
}

static void can_stub_a_string_sequence() {
    will_return(string_out, "hello");
    will_return(string_out, "goodbye");
    assert_equal(string_out(), "hello");
    assert_equal(string_out(), "goodbye");
}

static void integer_in(int i) {
    checked_integer(i);
}

static void expectation_confirmed() {
    expect(integer_in, 3);
    integer_in(3);
}

static void always_expected_keeps_affirming_parameter() {
    always_expect(integer_in, 3);
    integer_in(3);
    integer_in(3);
    integer_in(3);
}

static void expectation_sequence() {
    expect(integer_in, 1);
    expect(integer_in, 2);
    expect(integer_in, 3);
    integer_in(1);
    integer_in(2);
    integer_in(3);
}

static void string_in(char *s) {
    checked_string(s);
}

static void string_expectation_confirmed() {
    expect(string_in, "hello");
    string_in("hello");
}

static void string_expectation_sequence() {
    expect(string_in, "hello");
    expect(string_in, "goodbye");
    string_in("hello");
    string_in("goodbye");
}

static void mixed_parameters(int i, char *s) {
    checked_integer(i);
    checked_string(s);
}

static void confirming_multiple_parameters_multiple_times() {
    expect(mixed_parameters, 1, "Hello");
    expect(mixed_parameters, 2, "Goodbye");
    mixed_parameters(1, "Hello");
    mixed_parameters(2, "Goodbye");
}

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_stub_an_integer_return);
    add_test(suite, repeats_return_value_when_set_to_always);
    add_test(suite, can_stub_an_integer_return_sequence);
    add_test(suite, set_stub_just_to_be_cleared);
    add_test(suite, confirm_stub_is_reset_between_tests);
    add_test(suite, stub_uses_the_always_value_once_hit);
    add_test(suite, can_stub_a_string_return);
    add_test(suite, can_stub_a_string_sequence);
    add_test(suite, expectation_confirmed);
    add_test(suite, always_expected_keeps_affirming_parameter);
    add_test(suite, expectation_sequence);
    add_test(suite, string_expectation_confirmed);
    add_test(suite, string_expectation_sequence);
    add_test(suite, confirming_multiple_parameters_multiple_times);
    return suite;
}
