#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

/*
  These tests are only an attempt to exercise the whole public API of
  Cgreen to ensure that it compiles.  Attempting to run them will
  surely result in crashes, plagues and pestilence.  For unit tests
  see other tests.  */

#ifdef __cplusplus
using namespace cgreen;
#endif

// ASSERT
Ensure(assert_that_compiles) {
    assert_that(1 == 1);
    assert_that(1 == 1, is_true);
    assert_that_double(1, is_equal_to(1));
    significant_figures_for_assert_double_are(3);
}

// CONSTRAINTS
Ensure(constraints_compiles) {
    char array[5];
    char *string = (char *)"some string";

    assert_that(1, is_equal_to(1));
    assert_that(1, is_not_equal_to(1));
    assert_that(1, is_greater_than(1));
    assert_that(1, is_less_than(1));

    assert_that(array, is_equal_to_contents_of(array, 5));
    assert_that(array, is_not_equal_to_contents_of(array, 3));

    assert_that(string, is_equal_to_string(string));
    assert_that(string, is_not_equal_to_string(string));
    assert_that(string, contains_string(string));
    assert_that(string, does_not_contain_string(string));
    assert_that(string, begins_with_string(string));
}

static int int_stub(int parameter) {
    return mock(parameter);
}

// Mocks
Ensure(mocks_compiles) {
    always_expect(int_stub, will_return(1));
    always_expect(int_stub, will_set_contents_of_parameter(parameter, 1, sizeof(int)));
    never_expect(int_stub);
    expect(int_stub, when(parameter, is_equal_to(1)));
    int_stub(1);
}
