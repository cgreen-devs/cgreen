/* Note: the whole point of these tests is that they should fail!
   This makes it hard to think straight and to give the variables good
   names ;-) */

#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(ConstraintPrintout);
BeforeEach(ConstraintPrintout) {}
AfterEach(ConstraintPrintout) {}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_equal_to) {
    int fourty_five = 45, thirty_three = 33;
    assert_that(fourty_five, is_equal_to(thirty_three));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_not_equal_to) {
    int should_not_be_fourty_five = 45, forty_five = 45;
    assert_that(should_not_be_fourty_five, is_not_equal_to(forty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_greater_than) {
    int fourty_five = 45, thirty_three = 33;
    assert_that(thirty_three, is_greater_than(fourty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_less_than) {
    int fourty_five = 45, thirty_three = 33;
    assert_that(fourty_five, is_less_than(thirty_three));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_equal_to_contents_of) {
    int fourty_five[45] = {45, 44, 43}, thirty_three[33] = {45, 44, 33};
    assert_that(thirty_three, is_equal_to_contents_of(fourty_five, 55));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_not_equal_to_contents_of) {
    int fourty_five_and_up[3] = {45, 46, 47}, another_fourty_five_and_up[3] = {45, 46, 47};
    assert_that(fourty_five_and_up, is_not_equal_to_contents_of(another_fourty_five_and_up, 3));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_equal_to_string) {
    char *fourty_five = "this string is fortyfive", *thirty_three = "this string is thirtythree";
    assert_that(thirty_three, is_equal_to_string(fourty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_is_not_equal_to_string) {
    char *fourty_five = "this string is fourtyfive", *another_fourty_five = "this string is fourtyfive";
    assert_that(another_fourty_five, is_not_equal_to_string(fourty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_contains_string) {
    char *fourty_five = "fortyfive", *not_containing_fourty_five = "this text is thirtythree";
    assert_that(not_containing_fourty_five, contains_string(fourty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_does_not_contain_string) {
    char *contains_fourty_five = "this string is fourtyfive", *fourty_five = "fourtyfive";
    assert_that(contains_fourty_five, does_not_contain_string(fourty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_begins_with_string) {
    char *does_not_begin_with_fourty_five = "this string does not begin with fortyfive", *fourty_five = "fourtyfive";
    assert_that(does_not_begin_with_fourty_five, begins_with_string(fourty_five));
}

Ensure(ConstraintPrintout, shows_a_nice_message_for_assert_that) {
    assert_that(0 == 1);
}
