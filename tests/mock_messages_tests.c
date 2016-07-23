#include <cgreen/boxed_double.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/unit.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(Mocks);
BeforeEach(Mocks) {}
AfterEach(Mocks) {}

static int integer_out() {
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

