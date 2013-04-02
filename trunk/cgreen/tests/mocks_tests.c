#include <cgreen/boxed_double.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/unit.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static int integer_out() {
    return (int)mock();
}

Ensure(default_return_value_when_no_presets_for_loose_mock) {
    cgreen_mocks_are(loose_mocks);
    assert_that(integer_out(), is_equal_to(0));
}

Ensure(can_stub_an_integer_return) {
    expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(3));
}

Ensure(repeats_return_value_when_set_to_always) {
    always_expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(3));
    assert_that(integer_out(), is_equal_to(3));
}

Ensure(can_stub_an_integer_return_sequence) {
    expect(integer_out,
        will_return(1)
    );

    expect(integer_out,
        will_return(2)
    );

    expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(1));
    assert_that(integer_out(), is_equal_to(2));
    assert_that(integer_out(), is_equal_to(3));
}

Ensure(expectations_are_reset_between_tests_with_loose_mocks) {
    cgreen_mocks_are(loose_mocks);
    assert_that(integer_out(), is_equal_to(0));
}

static char *string_out() {
    return (char *)mock();
}

Ensure(can_stub_a_string_return) {
    expect(string_out, will_return("hello"));
    assert_string_equal(string_out(), "hello");
}

Ensure(can_stub_a_string_sequence) {
    expect(string_out, will_return("hello"));
    expect(string_out, will_return("goodbye"));

    assert_string_equal(string_out(), "hello");
    assert_string_equal(string_out(), "goodbye");
}

static void integer_in(int i) {
    mock(i);
}

Ensure(expecting_once_with_any_parameters) {
    expect(integer_in);
    integer_in(3);
}

Ensure(expecting_once_with_parameter_checks_that_parameter) {
    expect(integer_in, when(i, is_equal_to(3)));
    integer_in(3);
}

Ensure(always_expect_keeps_affirming_parameter) {
    always_expect(integer_in, when(i, is_equal_to(3)));
    integer_in(3);
    integer_in(3);
    integer_in(3);
}

Ensure(expect_a_sequence) {
    expect(integer_in, when(i, is_equal_to(1)));
    expect(integer_in, when(i, is_equal_to(2)));
    expect(integer_in, when(i, is_equal_to(3)));
    integer_in(1);
    integer_in(2);
    integer_in(3);
}

static void string_in(const char *s) {
    mock(s);
}

Ensure(string_expect_is_confirmed) {
    expect(string_in, when(s, is_equal_to_string("hello")));
    string_in("hello");
}

Ensure(string_contains_expectation_is_confirmed) {
    expect(string_in, when(s, contains_string("hello")));
    string_in("alice, hello");
}

Ensure(string_expect_is_confirmed_even_when_null) {
    expect(string_in, when(s, is_equal_to_string((char *)NULL)));
    string_in(NULL);
}

Ensure(string_expect_sequence) {
    expect(string_in, when(s, is_equal_to_string("hello")));
    expect(string_in, when(s, is_equal_to_string("goodbye")));
    string_in("hello");
    string_in("goodbye");
}

Ensure(expecting_once_with_non_null_parameter_checks_that_parameter) {
    expect(string_in, when(s, is_non_null));
    string_in("anything");
}

static void double_in(double d) {
    mock(box_double(d));
}

Ensure(double_expect_is_confirmed) {
    expect(double_in, when(d, is_equal_to_double(3.14)));
    double_in(3.14);
}

Ensure(double_expect_sequence) {
    expect(double_in, when(d, is_equal_to_double(1.0)));
    expect(double_in, when(d, is_equal_to_double(2.0)));
    double_in(1.0);
    double_in(2.0);
}

static void mixed_parameters(int i, const char *s) {
    mock(i, s);
}

Ensure(confirming_multiple_parameters_multiple_times) {
    expect(mixed_parameters, 
        when(i, is_equal_to(1)), 
        when(s, is_equal_to_string("Hello"))
    );
    expect(mixed_parameters, 
        when(i, is_equal_to(2)), 
        when(s, is_equal_to_string("Goodbye"))
    );

    mixed_parameters(1, "Hello");
    mixed_parameters(2, "Goodbye");
}

static int sample_mock(int i, const char *s) {
    return (int)mock(i, s);
}

Ensure(can_mock_full_function_call) {
    expect(sample_mock,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );

    assert_that(sample_mock(666, "devil"), is_equal_to(5));
}

Ensure(when_called_with_always_should_not_tally_counts) {
    always_expect(string_out,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );
}

Ensure(can_mock_full_sequence) {
    expect(sample_mock,
       will_return(5),
       when(i, is_equal_to(666)),
       when(s, is_equal_to_string("devil"))
    );

    expect(sample_mock,
        will_return(6),
        when(i, is_equal_to(667)),
        when(s, is_equal_to_string("beastie"))
    );

    assert_that(sample_mock(666, "devil"), is_equal_to(5));
    assert_that(sample_mock(667, "beastie"), is_equal_to(6));
}

Ensure(can_always_mock_full_function_call) {
    always_expect(sample_mock,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );

    assert_that(sample_mock(666, "devil"), is_equal_to(5));
    assert_that(sample_mock(666, "devil"), is_equal_to(5));
    assert_that(sample_mock(666, "devil"), is_equal_to(5));
}

typedef struct {
    double a;
    double b;
    const char *name;
} LargerThanIntptr;

static void out_param_mock(LargerThanIntptr* result) {
    mock(result);
}


Ensure(can_stub_an_out_parameter) {
    LargerThanIntptr actual = { 3.14, 6.66, "bob" };
    LargerThanIntptr local = { 4.13, 7.89, "alice" };

    expect(out_param_mock,
        will_set_contents_of_parameter(result, &actual, sizeof(LargerThanIntptr))
    );

    out_param_mock(&local);
    assert_that_double(actual.a, is_equal_to_double(local.a));
    assert_that_double(actual.b, is_equal_to_double(local.b));
    assert_that(actual.name, is_equal_to_string(local.name));
    assert_that(&local, is_equal_to_contents_of(&actual, sizeof(LargerThanIntptr)));
}

Ensure(none_emitted_when_learning_no_mocks) {
    cgreen_mocks_are(learning_mocks);
}

Ensure(pastable_code_emitted_by_learning_mocks) {
    cgreen_mocks_are(learning_mocks);
    string_out();
    string_out();
    integer_out();
    integer_out();
    string_out();
    integer_out();
}

/* Expected fail tests follow. */
/* TODO: put these in a separate suite and validate all tests in said suite fail during 'make check' */
/*
Ensure(can_declare_function_never_called) {
    never_expect(sample_mock);

    sample_mock(0, "");
}

Ensure(calls_beyond_expected_sequence_fail_when_mocks_are_strict) {
    expect(integer_out,
        will_return(1)
    );

    expect(integer_out,
        will_return(2)
    );

    expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(1));
    assert_that(integer_out(), is_equal_to(2));
    assert_that(integer_out(), is_equal_to(3));
    // expected fail
    assert_that(integer_out(), is_equal_to(3));
}

Ensure(failure_when_no_presets_for_default_strict_mock) {
    assert_that(integer_out(), is_equal_to(0));
}

Ensure(failure_reported_when_expect_after_never_expect_for_same_function) {
    never_expect(integer_out);

    expect(integer_out);
}

Ensure(failure_reported_when_expect_after_always_expect_for_same_function) {
    always_expect(integer_out,
        will_return(666)
    );

    expect(integer_out);
}

Ensure(single_uncalled_expectation_fails_tally) {
    expect(string_out,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );
}
*/

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, none_emitted_when_learning_no_mocks);
    add_test(suite, pastable_code_emitted_by_learning_mocks);
    add_test(suite, default_return_value_when_no_presets_for_loose_mock);
    add_test(suite, can_stub_an_integer_return);
    add_test(suite, repeats_return_value_when_set_to_always);
    add_test(suite, can_stub_an_integer_return_sequence);
    add_test(suite, expectations_are_reset_between_tests_with_loose_mocks);
    add_test(suite, can_stub_a_string_return);
    add_test(suite, can_stub_a_string_sequence);
    add_test(suite, expecting_once_with_any_parameters);
    add_test(suite, expecting_once_with_parameter_checks_that_parameter);
    add_test(suite, always_expect_keeps_affirming_parameter);
    add_test(suite, expect_a_sequence);
    add_test(suite, string_expect_is_confirmed);
    add_test(suite, string_expect_is_confirmed_even_when_null);
    add_test(suite, string_expect_sequence);
    add_test(suite, expecting_once_with_non_null_parameter_checks_that_parameter);
    add_test(suite, double_expect_is_confirmed);
    add_test(suite, double_expect_sequence);
    add_test(suite, confirming_multiple_parameters_multiple_times);
    add_test(suite, can_mock_full_function_call);
    add_test(suite, when_called_with_always_should_not_tally_counts);
    add_test(suite, can_mock_full_sequence);
    add_test(suite, can_always_mock_full_function_call);
    add_test(suite, can_stub_an_out_parameter);
    add_test(suite, string_contains_expectation_is_confirmed);

    /* expected failures. TODO: put these in a separate suite, as per comments above. */
/*    add_test(suite, failure_reported_when_expect_after_always_expect_for_same_function);
    add_test(suite, single_uncalled_expectation_fails_tally);
    add_test(suite, can_declare_function_never_called);
    add_test(suite, failure_reported_when_expect_after_never_expect_for_same_function);
    add_test(suite, failure_when_no_presets_for_default_strict_mock);
    add_test(suite, calls_beyond_expected_sequence_fail_when_mocks_are_strict);
*/
    return suite;
}
