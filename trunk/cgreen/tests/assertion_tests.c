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

Ensure one_should_assert_double_equal_to_one() {
    assert_double_equal(1, 1);
}

Ensure zero_should_assert_double_not_equal_to_one() {
    assert_double_not_equal(0, 1);
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
    add_test(suite, one_should_assert_double_equal_to_one);
    add_test(suite, zero_should_assert_double_not_equal_to_one);
    add_test(suite, double_differences_do_not_matter_past_significant_figures);
    add_test(suite, double_differences_matter_past_significant_figures);
    add_test(suite, double_assertions_can_have_custom_messages);
    add_test(suite, identical_string_copies_should_match);
    add_test(suite, case_different_strings_should_not_match);
    add_test(suite, null_string_should_only_match_another_null_string);
    add_test(suite, null_string_should_only_match_another_null_string_even_with_messages);
    return suite;
}
