#include <cgreen/cgreen.h>
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
