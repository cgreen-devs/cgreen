#include "../cgreen.h"
#include <stdlib.h>
#include <stdio.h>

void integer_one_should_assert_true() {
    assert_true(1);
}

void integer_zero_should_assert_false() {
    assert_false(0);
}

void one_should_assert_equal_to_one() {
    assert_equal(1, 1);
}

void zero_should_assert_not_equal_to_one() {
    assert_not_equal(0, 1);
}

void identical_string_copies_should_match() {
    assert_string_equal("Hello", "Hello");
}

void case_different_strings_should_not_match() {
    assert_string_not_equal("Hello", "hello");
}

void null_string_should_only_match_another_null_string() {
    assert_string_equal(NULL, NULL);
    assert_string_equal("", "");
    assert_string_not_equal(NULL, "");
    assert_string_not_equal("", NULL);
}

void null_string_should_only_match_another_null_string_even_with_messages() {
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
    add_test(suite, identical_string_copies_should_match);
    add_test(suite, case_different_strings_should_not_match);
    add_test(suite, null_string_should_only_match_another_null_string);
    add_test(suite, null_string_should_only_match_another_null_string_even_with_messages);
    return suite;
}
