#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/constraint.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(AssertionMessage);
BeforeEach(AssertionMessage) {}
AfterEach(AssertionMessage) {}


char something[42];
Ensure(AssertionMessage, for_comparing_content_with_negative_length) {
    assert_that(47, is_equal_to_contents_of(something, -4));
}

Ensure(AssertionMessage, return_value_constraints_are_not_allowed) {
    assert_that(0, will_return(1));
}

Ensure(AssertionMessage, for_compare_area_to_null) {
    char area[100];
    void *null_pointer = NULL;
    assert_that(area, is_equal_to_contents_of(null_pointer, 1));
}

Ensure(AssertionMessage, for_compare_null_to_area) {
    char area[100];
    assert_that(NULL, is_equal_to_contents_of(area, 1));
}

Ensure(AssertionMessage, for_actual_with_percent) {
    assert_that(strlen("%d"), is_equal_to(3)); /* Actually, it's not but should preserve '%' */
}

/* CAUTION: These two tests outputs the expected warning messages, but
   also fails the comparisons, obviously, since that is what they try
   to warn against. So the output of those failed constraints might
   change unintentionally. */
Ensure(AssertionMessage, for_using_double_constraints_with_assert_that) {
    assert_that(3, is_equal_to_double(3.1415926));
    assert_that(0, is_less_than_double(3.1415926));
    assert_that(7, is_greater_than_double(3.1415926));
}

Ensure(AssertionMessage, for_using_non_double_constraints_with_assert_that_double) {
    assert_that_double(3, is_equal_to(3));
}
