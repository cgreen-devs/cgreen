#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/constraint.h>
#include "constraint_internal.h"
#include <cgreen/message_formatting.h>
#include <stdbool.h>
#include <stddef.h>


Constraint static_is_non_null_constraint = {
    /* .type */ VALUE_COMPARER,
    /* .name */ "be non null",
    /* .destroy */ destroy_static_constraint,
    /* .compare */ compare_do_not_want_value,
    /* .test */ test_want,
    /* .format_failure_message_for */ failure_message_for,
    /* .actual_value_message */ "",
    /* .expected_value_message */ "",
    /* .expected_value */ {INTEGER, {0}, 0},
    /* .stored_value_name */ "null",
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0,
    /* .side_effect_callback */ NULL,
    /* .side_effect_data */ NULL
};

Constraint static_is_null_constraint = {
    /* .type */ VALUE_COMPARER,
    /* .name */ "be null",
    /* .destroy */ destroy_static_constraint,
    /* .compare */ compare_want_value,
    /* .test */ test_want,
    /* .format_failure_message_for */ failure_message_for,
    /* .actual_value_message */ "",
    /* .expected_value_message */ "",
    /* .expected_value */ {INTEGER, {(intptr_t)NULL}, 0},
    /* .stored_value_name */ "null",
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0,
    /* .side_effect_callback */ NULL,
    /* .side_effect_data */ NULL
};

Constraint static_is_false_constraint = {
    /* .type */ VALUE_COMPARER,
    /* .name */ "be false",
    /* .destroy */ destroy_static_constraint,
    /* .compare */ compare_want_value,
    /* .test */ test_want,
    /* .format_failure_message_for */ failure_message_for,
    /* .actual_value_message */ "",
    /* .expected_value_message */ "",
    /* .expected_value */ {INTEGER, {false}, 0},
    /* .stored_value_name */ "false",
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0,
    /* .side_effect_callback */ NULL,
    /* .side_effect_data */ NULL
};

Constraint static_is_true_constraint = {
    /* .type */ VALUE_COMPARER,
    /* .name */ "be true",
    /* .destroy */ destroy_static_constraint,
    /* .compare */ compare_do_not_want_value,
    /* .test */ test_want,
    /* .format_failure_message_for */ failure_message_for,
    /* .actual_value_message */ "",
    /* .expected_value_message */ "",
    /* .expected_value */ {INTEGER, {false}, 0},
    /* .stored_value_name */ "true",
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0,
    /* .side_effect_callback */ NULL,
    /* .side_effect_data */ NULL
};

Constraint *is_non_null = &static_is_non_null_constraint;
Constraint *is_null = &static_is_null_constraint;
Constraint *is_false = &static_is_false_constraint;
Constraint *is_true = &static_is_true_constraint;

/* vim: set ts=4 sw=4 et cindent: */
