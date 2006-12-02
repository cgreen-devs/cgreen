#include "../cgreen.h"
#include "../constraint.h"
#include <stdlib.h>

static void can_construct_and_destroy_an_want_constraint() {
    Constraint *any_old_want = want(label, 37);
    destroy_constraint(any_old_want);
}

static void equal_integers_compare_true_with_a_want_constraint() {
    Constraint *want_37 = want(label, 37);
    assert_equal(compare_constraint(want_37, 37), 1);
    destroy_constraint(want_37);
}

static void equal_pointers_compare_true_with_a_want_constraint() {
    Constraint *want_pointed_at_37 = want(label, (void *)37);
    assert_equal(compare_constraint(want_pointed_at_37, (void *)37), 1);
    assert_equal(compare_constraint(want_pointed_at_37, (void *)36), 0);
    destroy_constraint(want_pointed_at_37);
}

static void can_construct_and_destroy_an_want_string_constraint() {
    Constraint *any_old_string_want = want_string(label, "Hello");
    destroy_constraint(any_old_string_want);
}

static void can_compare_strings_as_equal() {
    Constraint *want_hello = want_string(label, "Hello");
    assert_equal(compare_constraint(want_hello, "Hello"), 1);
    assert_equal(compare_constraint(want_hello, "Goodbye"), 0);
    destroy_constraint(want_hello);
}

static void can_compare_null_strings_as_well_as_real_ones() {
    Constraint *want_hello = want_string(label, "Hello");
    assert_equal(compare_constraint(want_hello, NULL), 0);
    destroy_constraint(want_hello);
}

static void can_expect_null_strings_as_well_as_real_ones() {
    Constraint *want_hello = want_string(label, NULL);
    assert_equal(compare_constraint(want_hello, NULL), 1);
    assert_equal(compare_constraint(want_hello, "Hello"), 0);
    destroy_constraint(want_hello);
}

TestSuite *constraint_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_construct_and_destroy_an_want_constraint);
    add_test(suite, equal_integers_compare_true_with_a_want_constraint);
    add_test(suite, equal_pointers_compare_true_with_a_want_constraint);
    add_test(suite, can_construct_and_destroy_an_want_string_constraint);
    add_test(suite, can_compare_strings_as_equal);
    add_test(suite, can_compare_null_strings_as_well_as_real_ones);
    add_test(suite, can_expect_null_strings_as_well_as_real_ones);
    return suite;
}
