#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <stdlib.h>

static int integer_out() {
    return (int)mock();
}

Ensure no_errors_thrown_when_no_presets() {
    integer_out();
}

Ensure can_stub_an_integer_return() {
    will_return(integer_out, 3);
    assert_equal(integer_out(), 3);
}

Ensure repeats_return_value_when_set_to_always() {
    always_return(integer_out, 3);
    assert_equal(integer_out(), 3);
    assert_equal(integer_out(), 3);
}

Ensure can_stub_an_integer_return_sequence() {
    will_return(integer_out, 1);
    will_return(integer_out, 2);
    will_return(integer_out, 3);
    assert_equal(integer_out(), 1);
    assert_equal(integer_out(), 2);
    assert_equal(integer_out(), 3);
}

Ensure set_stub_just_to_be_cleared() {
    will_return(integer_out, 1);
}

Ensure confirm_stub_is_reset_between_tests() {
    will_return(integer_out, 2);
    assert_equal(integer_out(), 2);
}

Ensure stub_uses_the_always_value_once_hit() {
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
    return (char *)mock();
}

Ensure can_stub_a_string_return() {
    will_return(string_out, "hello");
    assert_string_equal(string_out(), "hello");
}

Ensure can_stub_a_string_sequence() {
    will_return(string_out, "hello");
    will_return(string_out, "goodbye");
    assert_string_equal(string_out(), "hello");
    assert_string_equal(string_out(), "goodbye");
}

static void integer_in(int i) {
    mock(i);
}

Ensure expecting_once_with_any_parameters() {
    expect(integer_in);
    integer_in(3);
}

Ensure expecting_once_with_parameter_checks_that_parameter() {
    expect(integer_in, want(i, 3));
    integer_in(3);
}

Ensure always_expect_keeps_affirming_parameter() {
    always_expect(integer_in, want(i, 3));
    integer_in(3);
    integer_in(3);
    integer_in(3);
}

Ensure expect_a_sequence() {
    expect(integer_in, want(i, 1));
    expect(integer_in, want(i, 2));
    expect(integer_in, want(i, 3));
    integer_in(1);
    integer_in(2);
    integer_in(3);
}

static void string_in(char *s) {
    mock(s);
}

Ensure string_expect_is_confirmed() {
    expect(string_in, want_string(s, "hello"));
    string_in("hello");
}

Ensure string_expect_is_confirmed_even_when_null() {
    expect(string_in, want_string(s, NULL));
    string_in(NULL);
}

Ensure string_expect_sequence() {
    expect(string_in, want_string(s, "hello"));
    expect(string_in, want_string(s, "goodbye"));
    string_in("hello");
    string_in("goodbye");
}

static void mixed_parameters(int i, char *s) {
    mock(i, s);
}

Ensure confirming_multiple_parameters_multiple_times() {
    expect(mixed_parameters, want(i, 1), want_string(s, "Hello"));
    expect(mixed_parameters, want(i, 2), want_string(s, "Goodbye"));
    mixed_parameters(1, "Hello");
    mixed_parameters(2, "Goodbye");
}

static int sample_mock(int i, char *s) {
    return (int)mock(i, s);
}

Ensure can_mock_full_function_call() {
    will_respond(sample_mock, 5, want(i, 666), want_string(s, "devil"));
    assert_equal(sample_mock(666, "devil"), 5);
}

Ensure when_called_with_always_should_not_tally_counts() {
    always_respond(sample_mock, 5, want(i, 666), want_string(s, "devil"));
}

Ensure can_mock_full_sequence() {
    will_respond(sample_mock, 5, want(i, 666), want_string(s, "devil"));
    will_respond(sample_mock, 6, want(i, 667), want_string(s, "beastie"));
    assert_equal(sample_mock(666, "devil"), 5);
    assert_equal(sample_mock(667, "beastie"), 6);
}

static void can_always_mock_full_function_call() {
    always_respond(sample_mock, 5, want(i, 666), want_string(s, "devil"));
    assert_equal(sample_mock(666, "devil"), 5);
    assert_equal(sample_mock(666, "devil"), 5);
    assert_equal(sample_mock(666, "devil"), 5);
}

Ensure can_declare_function_never_called() {
    expect_never(sample_mock);    
}

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, no_errors_thrown_when_no_presets);
    add_test(suite, can_stub_an_integer_return);
    add_test(suite, repeats_return_value_when_set_to_always);
    add_test(suite, can_stub_an_integer_return_sequence);
    add_test(suite, set_stub_just_to_be_cleared);
    add_test(suite, confirm_stub_is_reset_between_tests);
    add_test(suite, stub_uses_the_always_value_once_hit);
    add_test(suite, can_stub_a_string_return);
    add_test(suite, can_stub_a_string_sequence);
    add_test(suite, expecting_once_with_any_parameters);
    add_test(suite, expecting_once_with_parameter_checks_that_parameter);
    add_test(suite, always_expect_keeps_affirming_parameter);
    add_test(suite, expect_a_sequence);
    add_test(suite, string_expect_is_confirmed);
    add_test(suite, string_expect_is_confirmed_even_when_null);
    add_test(suite, string_expect_sequence);
    add_test(suite, confirming_multiple_parameters_multiple_times);
    add_test(suite, can_mock_full_function_call);
    add_test(suite, when_called_with_always_should_not_tally_counts);
    add_test(suite, can_mock_full_sequence);
    add_test(suite, can_always_mock_full_function_call);
    add_test(suite, can_declare_function_never_called);
    return suite;
}
