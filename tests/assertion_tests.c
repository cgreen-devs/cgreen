#include <cgreen/cgreen.h>
#include <cgreen/legacy.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Ensure(integer_one_should_assert_true) {
    assert_that(1, is_true);
}

Ensure(integer_two_should_assert_true) {
    assert_that(2, is_true);
}

Ensure(integer_zero_should_assert_false) {
    assert_that(0, is_false);
}

Ensure(one_should_assert_equal_to_one) {
    assert_that(1, is_equal_to(1));
}

Ensure(zero_should_assert_not_equal_to_one) {
    assert_that(0, is_not_equal_to(1));
}

Ensure(one_should_assert_long_equal_to_one) {
    long x = 1;
    long y = 1;
    assert_that(x, is_equal_to(y));
}

Ensure(zero_should_assert_long_not_equal_to_one) {
    long x = 0;
    long y = 1;
    assert_that(x, is_not_equal_to(y));
}

Ensure(one_should_assert_unsigned_long_equal_to_one) {
    unsigned long x = 1;
    unsigned long y = 1;
    assert_that(x, is_equal_to(y));
}

Ensure(zero_should_assert_unsigned_long_not_equal_to_one) {
    unsigned long x = 0;
    unsigned long y = 1;
    assert_that(x, is_not_equal_to(y));
}

Ensure(one_should_assert_long_long_equal_to_one) {
    long long x = 1;
    long long y = 1;
    assert_that(x, is_equal_to(y));
}

Ensure(zero_should_assert_long_long_not_equal_to_one) {
    long long x = 0;
    long long y = 1;
    assert_that(x, is_not_equal_to(y));
}

Ensure(one_should_assert_unsigned_long_long_equal_to_one) {
    unsigned long long x = 1;
    unsigned long long y = 1;
    assert_equal(x, y);
}

Ensure(zero_should_assert_unsigned_long_long_not_equal_to_one) {
    unsigned long long x = 0;
    unsigned long long y = 1;
    assert_that(x, is_not_equal_to(y));
}

Ensure(one_should_assert_short_equal_to_one) {
    short x = 1;
    short y = 1;
    assert_equal(x, y);
}

Ensure(zero_should_assert_short_not_equal_to_one) {
    short x = 0;
    short y = 1;
    assert_not_equal(x, y);
}

Ensure(one_should_assert_unsigned_short_equal_to_one) {
    unsigned short x = 1;
    unsigned short y = 1;
    assert_equal(x, y);
}

Ensure(zero_should_assert_unsigned_short_not_equal_to_one) {
    unsigned short x = 0;
    unsigned short y = 1;
    assert_not_equal(x, y);
}

Ensure(one_should_assert_char_equal_to_one) {
    char x = 1;
    char y = 1;
    assert_equal(x, y);
}

Ensure(zero_should_assert_char_not_equal_to_one) {
    char x = 0;
    char y = 1;
    assert_not_equal(x, y);
}

Ensure(one_should_assert_unsigned_char_equal_to_one) {
    unsigned char x = 1;
    unsigned char y = 1;
    assert_equal(x, y);
}

Ensure(zero_should_assert_unsigned_char_not_equal_to_one) {
    unsigned char x = 0;
    unsigned char y = 1;
    assert_not_equal(x, y);
}

Ensure(different_pointers_with_different_contents_should_not_assert_equal) {
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 666 };
    int other_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 666 };

    assert_that(data,
        is_equal_to_contents_of(other_data, sizeof(other_data))
    );
}

Ensure(different_pointers_with_same_contents_should_assert_equal) {
    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    int same_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

    assert_that(data,
        is_equal_to_contents_of(same_data, sizeof(same_data))
    );
}

Ensure(one_should_assert_float_equal_to_one) {
    float x = 1;
    float y = 1;
    assert_that_double(x, is_equal_to_double(y));
}

Ensure(zero_should_assert_float_not_equal_to_one) {
    float x = 0;
    float y = 1;
    assert_that_double(x, is_not_equal_to_double(y));
}

Ensure(one_should_assert_double_equal_to_one) {
    assert_that_double(1, is_equal_to_double(1));
}

Ensure(zero_should_assert_double_not_equal_to_one) {
    assert_double_not_equal(0, 1);
}

Ensure(zero_should_assert_double_equal_to_zero) {
    assert_double_equal(0, 0);
}

Ensure(one_should_assert_long_double_equal_to_one) {
    long double x = 1;
    long double y = 1;
    assert_that_double(x, is_equal_to_double(y));
}

Ensure(zero_should_assert_long_double_not_equal_to_one) {
    long double x = 0;
    long double y = 1;
    assert_that_double(x, is_not_equal_to_double(y));
}

Ensure(double_differences_do_not_matter_past_significant_figures) {
    significant_figures_for_assert_double_are(3);
    assert_that_double(1.113, is_equal_to_double(1.115));
    assert_that_double(1113, is_equal_to_double(1115));
    assert_that_double(1113000, is_equal_to_double(1115000));
    assert_that_double(1.1199999999, is_less_than_double(1.11));
    assert_that_double(1.11, is_greater_than_double(1.119999));
}

Ensure(can_check_equality_of_negative_floating_point_numbers) {
    significant_figures_for_assert_double_are(3);
    assert_that_double(-1.113, is_equal_to_double(-1.115));
    assert_that_double(-1.13, is_not_equal_to_double(-1.15));
}

Ensure(double_one_is_less_than_two) {
    assert_that_double(1.0, is_less_than_double(2.0));
}

Ensure(double_one_is_greater_than_zero) {
    assert_that_double(1.0, is_greater_than_double(0.0));
}

Ensure(double_can_compare_negative_numbers) {
    assert_that_double(1.0, is_greater_than_double(-10.0));
    assert_that_double(-2.0, is_less_than_double(1.0));
    assert_that_double(-21.0, is_less_than_double(-10.0));
    assert_that_double(-11.0, is_greater_than_double(-12.0));
}

Ensure(double_differences_matter_past_significant_figures) {
    significant_figures_for_assert_double_are(4);
    assert_that_double(1.113, is_not_equal_to_double(1.115));
    assert_that_double(1.113, is_less_than_double(1.115));
    assert_that_double(1.115, is_greater_than_double(1.113));
    assert_that_double(1113, is_not_equal_to_double(1115));
    assert_that_double(1113000, is_not_equal_to_double(1115000));
}

Ensure(double_assertions_can_have_custom_messages) {
    significant_figures_for_assert_double_are(3);

    /* this passes because the difference is below the set significant figure */
    assert_that_double(1.113, is_equal_to_double(1.115));
}

Ensure(identical_string_copies_should_match) {
    assert_that("Hello", is_equal_to_string("Hello"));
}

Ensure(case_different_strings_should_not_match) {
    assert_that("Hello", is_not_equal_to_string("hello"));
}

Ensure(identical_strings_contain_eachother) {
    assert_that("Hello", contains_string("Hello"));
}

Ensure(null_string_should_only_match_another_null_string) {
    assert_string_equal(NULL, NULL);
    assert_string_not_equal(NULL, "");
    assert_string_not_equal("", NULL);

    // fluent syntax technically doesn't allow NULL on left-hand side
    // we force the issue to make sure we are testing both syntaxes
    assert_that((intptr_t)NULL, is_equal_to_string((char *)NULL));
    assert_that((intptr_t)NULL, is_not_equal_to_string(""));
    assert_that("", is_not_equal_to_string((char *)NULL));
}

Ensure(null_string_should_only_match_another_null_string_even_with_messages) {
    assert_string_equal_with_message(NULL, NULL, "Oh dear");
    assert_string_equal_with_message("", "", "Oh dear");
    assert_string_not_equal_with_message(NULL, "", "Oh dear");
    assert_string_not_equal_with_message("", NULL, "Oh dear");
}

/*
 * TODO: make TestContext more easily injectable so we can trap the failure
 *       and compare the message. for now, manually uncomment and verify.
 */

Ensure(inspecting_contents_of_expected_value_of_null_produces_nice_error_message) {
//	char buffer[0];
//	const int NON_ZERO = !0;
//	assert_that(buffer, is_equal_to_contents_of(NULL, NON_ZERO));
}

Ensure(inspecting_contents_of_actual_value_of_null_produces_nice_error_message) {
//	char buffer[0];
//	const int NON_ZERO = !0;
//	assert_that(NULL, is_equal_to_contents_of(buffer, NON_ZERO));
}

Ensure(inspecting_contents_of_with_zero_size_produces_nice_error_message) {
//	char buffer[0];
//	const int ZERO = 0;
//	assert_that(buffer, is_equal_to_contents_of(buffer, ZERO));
}

Ensure(fail_reports_message) {
//    fail("\n*** correctly failing ***\n");
}

Ensure(return_value_constraints_are_not_allowed) {
//    assert_that(0, will_return(1));
}

TestSuite *assertion_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test(suite, integer_one_should_assert_true);
    add_test(suite, integer_two_should_assert_true);
    add_test(suite, integer_zero_should_assert_false);
    add_test(suite, one_should_assert_equal_to_one);
    add_test(suite, zero_should_assert_not_equal_to_one);
    add_test(suite, one_should_assert_long_equal_to_one);
    add_test(suite, zero_should_assert_long_not_equal_to_one);
    add_test(suite, one_should_assert_unsigned_long_equal_to_one);
    add_test(suite, zero_should_assert_unsigned_long_not_equal_to_one);
    add_test(suite, one_should_assert_long_long_equal_to_one);
    add_test(suite, zero_should_assert_long_long_not_equal_to_one);
    add_test(suite, one_should_assert_unsigned_long_long_equal_to_one);
    add_test(suite, zero_should_assert_unsigned_long_long_not_equal_to_one);
    add_test(suite, one_should_assert_short_equal_to_one);
    add_test(suite, zero_should_assert_short_not_equal_to_one);
    add_test(suite, one_should_assert_unsigned_short_equal_to_one);
    add_test(suite, zero_should_assert_unsigned_short_not_equal_to_one);
    add_test(suite, one_should_assert_char_equal_to_one);
    add_test(suite, zero_should_assert_char_not_equal_to_one);
    add_test(suite, one_should_assert_unsigned_char_equal_to_one);
    add_test(suite, zero_should_assert_unsigned_char_not_equal_to_one);
    add_test(suite, different_pointers_with_different_contents_should_not_assert_equal);
    add_test(suite, different_pointers_with_same_contents_should_assert_equal);
    add_test(suite, one_should_assert_float_equal_to_one);
    add_test(suite, zero_should_assert_float_not_equal_to_one);
    add_test(suite, one_should_assert_double_equal_to_one);
    add_test(suite, zero_should_assert_double_not_equal_to_one);
    add_test(suite, zero_should_assert_double_equal_to_zero);
    add_test(suite, one_should_assert_long_double_equal_to_one);
    add_test(suite, zero_should_assert_long_double_not_equal_to_one);
    add_test(suite, double_differences_do_not_matter_past_significant_figures);
    add_test(suite, can_check_equality_of_negative_floating_point_numbers);
    add_test(suite, double_one_is_less_than_two);
    add_test(suite, double_one_is_greater_than_zero);
    add_test(suite, double_can_compare_negative_numbers);
    add_test(suite, double_differences_matter_past_significant_figures);
    add_test(suite, double_assertions_can_have_custom_messages);
    add_test(suite, identical_string_copies_should_match);
    add_test(suite, case_different_strings_should_not_match);
    add_test(suite, identical_strings_contain_eachother);
    add_test(suite, null_string_should_only_match_another_null_string);
    add_test(suite, null_string_should_only_match_another_null_string_even_with_messages);
    add_test(suite, inspecting_contents_of_expected_value_of_null_produces_nice_error_message);
    add_test(suite, inspecting_contents_of_actual_value_of_null_produces_nice_error_message);
    add_test(suite, inspecting_contents_of_with_zero_size_produces_nice_error_message);
    add_test(suite, fail_reports_message);
    add_test(suite, return_value_constraints_are_not_allowed);
    return suite;
}
