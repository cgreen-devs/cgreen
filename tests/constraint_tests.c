#include <cgreen/cgreen.h>
#include <cgreen/constraint.h>
#include <stdlib.h>

Ensure can_construct_and_destroy_an_want_constraint() {
    Constraint *any_old_want = want(label, 37);
    destroy_constraint(any_old_want);
}

Ensure parameter_name_gives_true_if_matching() {
    Constraint *any_old_want = want(label, 37);
    assert_equal(is_constraint_parameter(any_old_want, "wrong_label"), 0);
    assert_equal(is_constraint_parameter(any_old_want, "label"), 1);
    destroy_constraint(any_old_want);
}

Ensure equal_integers_compare_true_with_a_want_constraint() {
    Constraint *want_37 = want(label, 37);
    assert_equal(compare_constraint(want_37, 37), 1);
    destroy_constraint(want_37);
}

Ensure equal_pointers_compare_true_with_a_want_constraint() {
    Constraint *want_pointed_at_37 = want(label, (void *)37);
    assert_equal(compare_constraint(want_pointed_at_37, (void *)37), 1);
    assert_equal(compare_constraint(want_pointed_at_37, (void *)36), 0);
    destroy_constraint(want_pointed_at_37);
}

Ensure can_construct_and_destroy_a_want_string_constraint() {
    Constraint *any_old_string_want = want_string(label, "Hello");
    destroy_constraint(any_old_string_want);
}

Ensure can_compare_strings_as_equal() {
    Constraint *want_hello = want_string(label, "Hello");
    assert_equal(compare_constraint(want_hello, "Hello"), 1);
    assert_equal(compare_constraint(want_hello, "Goodbye"), 0);
    destroy_constraint(want_hello);
}

Ensure can_compare_null_strings_as_well_as_real_ones() {
    Constraint *want_hello = want_string(label, "Hello");
    assert_equal(compare_constraint(want_hello, NULL), 0);
    destroy_constraint(want_hello);
}

Ensure can_expect_null_strings_as_well_as_real_ones() {
    Constraint *want_hello = want_string(label, NULL);
    assert_equal(compare_constraint(want_hello, NULL), 1);
    assert_equal(compare_constraint(want_hello, "Hello"), 0);
    destroy_constraint(want_hello);
}

Ensure equal_doubles_compare_true_with_a_want_double_constraint() {
    Constraint *want_37 = want_double(label, 37.0);
    assert_equal(compare_constraint(want_37, box_double(37.0)), 1);
    destroy_constraint(want_37);
}

Ensure unequal_doubles_compare_false_with_a_want_double_constraint() {
    Constraint *want_37 = want_double(label, 37.0);
    assert_equal(compare_constraint(want_37, box_double(36.0)), 0);
    destroy_constraint(want_37);
}

Ensure constraints_on_doubles_respect_significant_figure_setting() {
	significant_figures_for_assert_double_are(2);
    Constraint *want_337 = want_double(label, 337.0);
    assert_equal(compare_constraint(want_337, box_double(339.0)), 1);
	significant_figures_for_assert_double_are(3);
    assert_equal(compare_constraint(want_337, box_double(339.0)), 0);
    destroy_constraint(want_337);
}

typedef struct {
	char* value;
	unsigned int length;
} String;

static int is_non_empty_string(const void* other) {
	String* our = (String*)other;
	return our->length != 0;
}

Ensure unequal_structs_with_same_value_for_specific_field_compare_true() {
    String name;
    name.value = "bob";
    name.length = 3;
    
    Constraint *string_constraint = with(name, is_non_empty_string);

    assert_equal(compare_constraint(string_constraint, is_non_empty_string), 1);

    destroy_constraint(string_constraint);

    name.value = "tim";
    name.length = 3;
    
    string_constraint = with(name, is_non_empty_string);
    assert_equal(compare_constraint(string_constraint, is_non_empty_string), 1);

    destroy_constraint(string_constraint);
}


TestSuite *constraint_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_construct_and_destroy_an_want_constraint);
    add_test(suite, parameter_name_gives_true_if_matching);
    add_test(suite, equal_integers_compare_true_with_a_want_constraint);
    add_test(suite, equal_pointers_compare_true_with_a_want_constraint);
    add_test(suite, can_construct_and_destroy_a_want_string_constraint);
    add_test(suite, can_compare_strings_as_equal);
    add_test(suite, can_compare_null_strings_as_well_as_real_ones);
    add_test(suite, can_expect_null_strings_as_well_as_real_ones);
    add_test(suite, equal_doubles_compare_true_with_a_want_double_constraint);
    add_test(suite, unequal_doubles_compare_false_with_a_want_double_constraint);
    add_test(suite, constraints_on_doubles_respect_significant_figure_setting);
    add_test(suite, unequal_structs_with_same_value_for_specific_field_compare_true);
    return suite;
}
