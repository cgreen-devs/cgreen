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

Ensure(Mocks, single_uncalled_expectation_fails_tally) {
    expect(string_out,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );
}
