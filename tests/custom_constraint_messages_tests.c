#include <cgreen/cgreen.h>
#include <cgreen/message_formatting.h>
#include "constraint_internal.h"
#include "cgreen_value_internal.h"
#include "utils.h"

Describe(CustomConstraint);
BeforeEach(CustomConstraint) {}
AfterEach(CustomConstraint) {}


/* Example 1 from documentation : is_bigger_than_5()

   No argument to the constraint.

 */

bool compare_want_greater_than_5(Constraint *constraint, CgreenValue actual) {
    (void)constraint;
    return actual.value.integer_value > 5;
}

Constraint static_is_bigger_than_5 = {
        /* .type */ VALUE_COMPARER,
        /* .name */ "be bigger than 5",
        /* .destroy */ destroy_static_constraint,
        /* .compare */ compare_want_greater_than_5,
        /* .test */ test_want,
        /* .format_failure_message_for */ failure_message_for,
        /* .actual_value_message */ "",
        /* .expected_value_message */ "",
        /* .expected_value */ {INTEGER, {5}, 0},
        /* .stored_value_name */ "null",
        /* .parameter_name */ NULL,
        /* .size_of_stored_value */ 0,
        /* .side_effect_callback */ NULL,
        /* .side_effect_data */ NULL
};

/* Remember: failing tests to get output */
Ensure(CustomConstraint, custom_constraint_using_static_function) {
    Constraint * is_bigger_than_5 = &static_is_bigger_than_5;
    assert_that(1, is_bigger_than_5);
}


/* Example 2 from documentation : is_smaller_than()

   Standard data type argument to constraint.

 */

bool compare_want_smaller_value(Constraint *constraint, CgreenValue actual) {
    return actual.value.integer_value < constraint->expected_value.value.integer_value ;
}

Constraint *create_smaller_than_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint();

    constraint->expected_value = make_cgreen_integer_value(expected_value);
    constraint->expected_value_name = string_dup(expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_want_smaller_value;
    constraint->execute = &test_want;
    constraint->name = "be smaller than";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}
#define is_smaller_than(value) create_smaller_than_constraint(value, #value)


Ensure(CustomConstraint, custom_constraint_using_a_function_with_arguments_function) {
    assert_that(19, is_smaller_than(10));
}


/* Example 3 from documentation : can_fit_in_box()

   Using custom data types.

*/
typedef struct Box {
    int id;
    int size;
} Box;

typedef struct Piece {
    int id;
    int size;
} Piece;

bool compare_piece_and_box_size(Constraint *constraint, CgreenValue actual) {
    return ((Piece *)actual.value.pointer_value)->size
        < ((Box*)constraint->expected_value.value.pointer_value)->size ;
}

static void test_fit_piece(Constraint *constraint, const char *function_name, CgreenValue actual,
                           const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Piece [%f], does not fit in [%f] in function [%s] parameter [%s]",
            ((Piece *)constraint->expected_value.value.pointer_value)->id,
            ((Box *)actual.value.pointer_value)->id,
            function_name,
            constraint->parameter_name);
}

Constraint *create_piece_fit_in_box_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint();

    constraint->expected_value = make_cgreen_pointer_value((void*)expected_value);
    constraint->expected_value_name = string_dup(expected_value_name);
    constraint->type = CONTENT_COMPARER;

    constraint->compare = &compare_piece_and_box_size;
    constraint->execute = &test_fit_piece;
    constraint->name = "fit in box";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}
#define can_fit_in_box(box) create_piece_fit_in_box_constraint((intptr_t)box, #box)

Ensure(CustomConstraint, more_complex_custom_constraint_function) {
    Box box1 = {.id = 1, .size = 5};
    Piece piece99 = {.id = 99, .size = 6};
    assert_that(&piece99, can_fit_in_box(&box1));
}
