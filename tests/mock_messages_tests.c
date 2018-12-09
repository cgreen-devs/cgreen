#include <cgreen/boxed_double.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/unit.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

/* NOTE: These tests are not designed to pass, they are run to output
   messages which need to be compared to some golden output */

Describe(Mocks);
BeforeEach(Mocks) {}
AfterEach(Mocks) {}

static int integer_out(void) {
    return (int)mock();
}

static char *string_out(int p1) {
    return (char *)mock(p1);
}

static int sample_mock(int i, const char *s) {
    return (int)mock(i, s);
}

Ensure(Mocks, can_declare_function_never_called) {
    never_expect(sample_mock);

    sample_mock(0, "");
}

Ensure(Mocks, when_never_expected_mock_function_is_not_called_pass_counter_increments_by_one) {
    never_expect(sample_mock);
}

Ensure(Mocks, calls_beyond_expected_sequence_fail_when_mocks_are_strict) {
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

// These are tentative solutions to mocks returning doubles and
// trapping type errors when they do.

/* xEnsure(Mocks, traps_double_return_value_for_int_mock) { */
/*     expect(integer_out, will_return_double(3.1415926)); */
/*     integer_out(); */
/* } */

/* static double double_out() { */
/*     return mock_double(); */
/* } */

/* xEnsure(Mocks, traps_int_return_value_for_double_mock) { */
/*     expect(double_out, will_return_double(3.1415926)); */
/*     assert_that(double_out(), is_equal_to_double(3.1415926)); */
/* } */

Ensure(Mocks, failure_when_no_presets_for_default_strict_mock) {
    assert_that(integer_out(), is_equal_to(0));
}

Ensure(Mocks, failure_reported_when_expect_after_never_expect_for_same_function) {
    never_expect(integer_out);

    expect(integer_out);
}

Ensure(Mocks, failure_reported_when_expect_after_always_expect_for_same_function) {
    always_expect(integer_out,
        will_return(666)
    );

    expect(integer_out);
}

Ensure(Mocks, reports_multiple_always_expect) {
    always_expect(integer_out);
    always_expect(integer_out);
}

Ensure(Mocks, reports_always_expect_after_never_expect_for_same_function) {
    never_expect(integer_out);
    always_expect(integer_out);
}

Ensure(Mocks, reports_never_expect_after_always_expect_for_same_function) {
    always_expect(integer_out);
    never_expect(integer_out);
}

Ensure(Mocks, reports_multiple_never_expect) {
    never_expect(integer_out);
    never_expect(integer_out);
}

Ensure(Mocks, single_uncalled_expectation_fails_tally) {
    expect(string_out,
           will_return(5),
           when(i, is_equal_to(666)),
           when(s, is_equal_to_string("devil"))
           );
}


Ensure(Mocks, should_detect_two_unfulfilled_expectations_on_unknown_functions) {
    expect(f1, will_return('a'));
    expect(f2, will_return('b'));
}

Ensure(Mocks, should_detect_two_unfulfilled_expectations_without_constraints_on_unknown_functions) {
    expect(f1);
    expect(f2);
}

static double double_out(void) {
    return (double)mock();
}

static void double_in(double in) {
    mock(box_double(in));
}

/* TODO: this will not report anything until v2 */
/* Why? */
xEnsure(Mocks, reports_mock_cannot_return_double) {
    expect(double_out, will_return_double(4.123));
    double_out();
}

Ensure(Mocks, learning_mocks_emit_none_when_learning_no_mocks) {
    cgreen_mocks_are(learning_mocks);
}

Ensure(Mocks, learning_mocks_emit_pastable_code) {
    cgreen_mocks_are(learning_mocks);
    string_out(1);
    string_out(2);
    integer_out();
    integer_out();
    string_out(3);
    integer_out();
}

Ensure(Mocks, can_learn_double_expects) {
    cgreen_mocks_are(learning_mocks);
    double_in(3.14);
}

/* It would be very nice if learning mocks could survive unexpected
   terminations but since the child process is already gone when the
   parent process detects this we need to either start catching
   exceptions in the child or communicate the learned mock calls to
   the parent some other way.
*/
xEnsure(Mocks, learning_mocks_survive_termination) {
    int *ip = 0;
    cgreen_mocks_are(learning_mocks);
    string_out(1);
    *ip = 0;
}

static void simple_mocked_function(int first, int second) {
    mock(first, second);
}

Ensure(Mocks, constraint_number_of_calls_when_not_called_enough_times) {
    expect(simple_mocked_function, times(2));
    simple_mocked_function(1, 2);
}

Ensure(Mocks, constraint_number_of_calls_out_of_order_expectations_fail) {
    expect(simple_mocked_function, when(first, is_equal_to(1)), times(1));
    expect(simple_mocked_function, when(first, is_equal_to(2)), times(1));
    simple_mocked_function(2, 2);
    simple_mocked_function(1, 2);
}
