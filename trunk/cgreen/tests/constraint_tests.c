#include "../cgreen.h"
#include "../constraint.h"
#include <stdlib.h>

static void can_construct_and_destroy_an_is_constraint() {
    CgreenConstraint *any_old_is = is(label, 37);
    destroy_constraint(any_old_is);
}

static void equal_integers_compare_true_with_an_is_constraint() {
    CgreenConstraint *is_37 = is(label, 37);
    assert_equal(compare_constraint(is_37, 37), 1);
    destroy_constraint(is_37);
}

static void equal_pointers_compare_true_with_an_is_constraint() {
    CgreenConstraint *is_pointed_at_37 = is(label, (void *)37);
    assert_equal(compare_constraint(is_pointed_at_37, (void *)37), 1);
    assert_equal(compare_constraint(is_pointed_at_37, (void *)36), 0);
    destroy_constraint(is_pointed_at_37);
}

static void can_construct_and_destroy_an_is_string_constraint() {
    CgreenConstraint *any_old_string_is = is_string(label, "Hello");
    destroy_constraint(any_old_string_is);
}

static void can_compare_strings_as_equal() {
    CgreenConstraint *is_hello = is_string(label, "Hello");
    assert_equal(compare_constraint(is_hello, "Hello"), 1);
    assert_equal(compare_constraint(is_hello, "Goodbye"), 0);
    destroy_constraint(is_hello);
}

static void can_compare_null_strings_as_well_as_real_ones() {
    CgreenConstraint *is_hello = is_string(label, "Hello");
    assert_equal(compare_constraint(is_hello, NULL), 0);
    destroy_constraint(is_hello);
}

TestSuite *constraint_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_construct_and_destroy_an_is_constraint);
    add_test(suite, equal_integers_compare_true_with_an_is_constraint);
    add_test(suite, equal_pointers_compare_true_with_an_is_constraint);
    add_test(suite, can_construct_and_destroy_an_is_string_constraint);
    add_test(suite, can_compare_strings_as_equal);
    add_test(suite, can_compare_null_strings_as_well_as_real_ones);
    return suite;
}
