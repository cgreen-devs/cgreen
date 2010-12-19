#include <cgreen/boxed_double.h>
#include <cgreen/cgreen.h>
#include <cgreen/constraint.h>
#include <stdlib.h>

Ensure(default_destroy_clears_state) {
    Constraint *constraint = create_parameter_constraint_for("parameter name");
    destroy_constraint(constraint);

/* these checks correctly trip valgrind's use-after-free check, so
 * uncomment and check manually

    assert_equal(constraint->name, NULL);
    assert_equal(constraint->parameter, NULL);
    assert_equal(constraint->storedValue, NULL);
    assert_equal(constraint->test, NULL);
    assert_equal(constraint->compare, NULL);
    assert_equal(constraint->destroy, NULL);
 */
}

Ensure(parameter_name_matches_correctly) {
    Constraint *constraint = create_parameter_constraint_for("label");

    assert_false(constraint_is_for_parameter(constraint, "wrong_label"));
    assert_true(constraint_is_for_parameter(constraint, "label"));

    destroy_constraint(constraint);
}

Ensure(compare_is_correct_when_using_integers) {
    Constraint *is_equal_to_37 = create_equal_to_intptr_constraint(37);

    assert_true(compare_constraint(is_equal_to_37, 37));
    assert_false(compare_constraint(is_equal_to_37, 36));

    destroy_constraint(is_equal_to_37);
}

Ensure(string_constraint_destroy_clears_state) {
    Constraint *string_constraint = create_equal_to_string_constraint("Hello");
    destroy_constraint(string_constraint);

/* these checks correctly trip valgrind's use-after-free check, so
 * uncomment and check manually

    assert_equal(string_constraint->name, NULL);
    assert_equal(string_constraint->parameter, NULL);
    assert_equal(string_constraint->storedValue, NULL);
    assert_equal(string_constraint->test, NULL);
    assert_equal(string_constraint->compare, NULL);
    assert_equal(string_constraint->destroy, NULL);
 */
}

Ensure(matching_strings_as_equal) {
    Constraint *equals_string_hello_constraint = create_equal_to_string_constraint("Hello");

    assert_true(compare_constraint(equals_string_hello_constraint, "Hello"));
    assert_false(compare_constraint(equals_string_hello_constraint, "Goodbye"));

    destroy_constraint(equals_string_hello_constraint);
}

Ensure(matching_null_string_against_non_null_string) {
    Constraint *equals_string_hello_constraint = create_equal_to_string_constraint("Hello");

    assert_false(compare_constraint(equals_string_hello_constraint, NULL));

    destroy_constraint(equals_string_hello_constraint);
}

Ensure(matching_against_null_string) {
    Constraint *equals_null_string_constraint = create_equal_to_string_constraint(NULL);

    assert_true(compare_constraint(equals_null_string_constraint, NULL));
    assert_false(compare_constraint(equals_null_string_constraint, "Hello"));

    destroy_constraint(equals_null_string_constraint);
}

Ensure(matching_doubles_as_equal_with_default_significance) {
    Constraint *equal_to_double_37 = create_equal_to_double_constraint(37.0);

    intptr_t boxed_37 = box_double(37.0);
    intptr_t boxed_36 = box_double(36.0);
    assert_true(compare_constraint(equal_to_double_37, boxed_37));
    assert_false(compare_constraint(equal_to_double_37, boxed_36));

    destroy_constraint(equal_to_double_37);
    (void)unbox_double(boxed_37);
    (void)unbox_double(boxed_36);
}

Ensure(matching_doubles_respects_significant_figure_setting) {
    Constraint *want_337 = create_equal_to_double_constraint(337.0);
    intptr_t boxed_339 = box_double(339.0);

    significant_figures_for_assert_double_are(2);
    assert_true(compare_constraint(want_337, boxed_339));

    significant_figures_for_assert_double_are(3);
    assert_false(compare_constraint(want_337, boxed_339));

    destroy_constraint(want_337);
    (void)unbox_double(boxed_339);
}

// TODO: convert to create_constraint_for_comparator("non-empty string", &is_non_empty_string);
//typedef struct {
//	char* value;
//	unsigned int length;
//} String;
//
//static int is_non_empty_string(const void* other) {
//	String* our = (String*)other;
//	return our->length != 0;
//}
//
//Ensure(unequal_structs_with_same_value_for_specific_field_compare_true) {
//    String name;
//    name.value = "bob";
//    name.length = 3;
//
//    Constraint *string_constraint = with(name, is_non_empty_string);
//
//    assert_equal(compare_constraint(string_constraint, is_non_empty_string), 1);
//
//    destroy_constraint(string_constraint);
//
//    name.value = "tim";
//    name.length = 3;
//
//    string_constraint = with(name, is_non_empty_string);
//    assert_equal(compare_constraint(string_constraint, is_non_empty_string), 1);
//
//    destroy_constraint(string_constraint);
//}


TestSuite *constraint_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, default_destroy_clears_state);
    add_test(suite, parameter_name_matches_correctly);
    add_test(suite, compare_is_correct_when_using_integers);
    add_test(suite, string_constraint_destroy_clears_state);
    add_test(suite, matching_strings_as_equal);
    add_test(suite, matching_null_string_against_non_null_string);
    add_test(suite, matching_against_null_string);
    add_test(suite, matching_doubles_as_equal_with_default_significance);
    add_test(suite, matching_doubles_respects_significant_figure_setting);
//    add_test(suite, unequal_structs_with_same_value_for_specific_field_compare_true);
    return suite;
}
