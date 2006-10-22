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

static void integer_in(int i) {
    checked_integer(i);
}

static void expectation_confirmed() {
    expect(integer_in, 3);
    integer_in(3);
}

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_stub_an_integer_return);
    add_test(suite, repeats_return_value_when_set_to_always);
    add_test(suite, can_stub_an_integer_return_sequence);
    add_test(suite, set_stub_just_to_be_cleared);
    add_test(suite, confirm_stub_is_reset_between_tests);
    add_test(suite, stub_uses_the_always_value_once_hit);
    add_test(suite, expectation_confirmed);
    return suite;
}
