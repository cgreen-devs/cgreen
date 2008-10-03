#include <cgreen/cgreen.h>
#include <stdlib.h>
#include <stdio.h>

Ensure integer_one_should_assert_true() {
    assert_true(1);
}

Ensure integer_zero_should_assert_false() {
    assert_false(0);
}

Ensure one_should_assert_equal_to_one() {
    assert_equal(1, 1);
}

Ensure zero_should_assert_not_equal_to_one() {
    assert_not_equal(0, 1);
}

Ensure one_should_assert_long_equal_to_one() {
	long x = 1;
	long y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_long_not_equal_to_one() {
	long x = 0;
	long y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_unsigned_long_equal_to_one() {
	unsigned long x = 1;
	unsigned long y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_unsigned_long_not_equal_to_one() {
	unsigned long x = 0;
	unsigned long y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_long_long_equal_to_one() {
	long long x = 1;
	long long y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_long_long_not_equal_to_one() {
	long long x = 0;
	long long y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_unsigned_long_long_equal_to_one() {
	unsigned long long x = 1;
	unsigned long long y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_unsigned_long_long_not_equal_to_one() {
	unsigned long long x = 0;
	unsigned long long y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_short_equal_to_one() {
	short x = 1;
	short y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_short_not_equal_to_one() {
	short x = 0;
	short y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_unsigned_short_equal_to_one() {
	unsigned short x = 1;
	unsigned short y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_unsigned_short_not_equal_to_one() {
	unsigned short x = 0;
	unsigned short y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_char_equal_to_one() {
	char x = 1;
	char y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_char_not_equal_to_one() {
	char x = 0;
	char y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_unsigned_char_equal_to_one() {
	unsigned char x = 1;
	unsigned char y = 1;
    assert_equal(x, y);
}

Ensure zero_should_assert_unsigned_char_not_equal_to_one() {
	unsigned char x = 0;
	unsigned char y = 1;
    assert_not_equal(x, y);
}

Ensure one_should_assert_float_equal_to_one() {
	float x = 1;
	float y = 1;
    assert_double_equal(x, y);
}

Ensure zero_should_assert_float_not_equal_to_one() {
	float x = 0;
	float y = 1;
    assert_double_not_equal(x, y);
}

Ensure one_should_assert_double_equal_to_one() {
    assert_double_equal(1, 1);
}

Ensure zero_should_assert_double_not_equal_to_one() {
    assert_double_not_equal(0, 1);
}

Ensure one_should_assert_long_double_equal_to_one() {
	long double x = 1;
	long double y = 1;
    assert_double_equal(x, y);
}

Ensure zero_should_assert_long_double_not_equal_to_one() {
	long double x = 0;
	long double y = 1;
    assert_double_not_equal(x, y);
}

Ensure double_differences_do_not_matter_past_significant_figures() {
	significant_figures_for_assert_double_are(3);
	assert_double_equal(1.113, 1.115);
	assert_double_equal(1113, 1115);
	assert_double_equal(1113000, 1115000);
}

Ensure double_differences_matter_past_significant_figures() {
	significant_figures_for_assert_double_are(4);
	assert_double_not_equal(1.113, 1.115);
	assert_double_not_equal(1113, 1115);
	assert_double_not_equal(1113000, 1115000);
}

Ensure double_assertions_can_have_custom_messages() {
	significant_figures_for_assert_double_are(3);
	assert_double_equal_with_message(1.113, 1.115, "This should pass");
}

Ensure identical_string_copies_should_match() {
    assert_string_equal("Hello", "Hello");
}

Ensure case_different_strings_should_not_match() {
    assert_string_not_equal("Hello", "hello");
}

Ensure null_string_should_only_match_another_null_string() {
    assert_string_equal(NULL, NULL);
    assert_string_equal("", "");
    assert_string_not_equal(NULL, "");
    assert_string_not_equal("", NULL);
}

Ensure null_string_should_only_match_another_null_string_even_with_messages() {
    assert_string_equal_with_message(NULL, NULL, "Oh dear");
    assert_string_equal_with_message("", "", "Oh dear");
    assert_string_not_equal_with_message(NULL, "", "Oh dear");
    assert_string_not_equal_with_message("", NULL, "Oh dear");
}

TestSuite *assertion_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, integer_one_should_assert_true);
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
    add_test(suite, one_should_assert_float_equal_to_one);
    add_test(suite, zero_should_assert_float_not_equal_to_one);
    add_test(suite, one_should_assert_double_equal_to_one);
    add_test(suite, zero_should_assert_double_not_equal_to_one);
    add_test(suite, one_should_assert_long_double_equal_to_one);
    add_test(suite, zero_should_assert_long_double_not_equal_to_one);
    add_test(suite, double_differences_do_not_matter_past_significant_figures);
    add_test(suite, double_differences_matter_past_significant_figures);
    add_test(suite, double_assertions_can_have_custom_messages);
    add_test(suite, identical_string_copies_should_match);
    add_test(suite, case_different_strings_should_not_match);
    add_test(suite, null_string_should_only_match_another_null_string);
    add_test(suite, null_string_should_only_match_another_null_string_even_with_messages);
    return suite;
}
