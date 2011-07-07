#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/constraint.h>

#ifdef __cplusplus
namespace cgreen {
#endif

Constraint static_is_non_null_constraint = {
    /* .type */ PARAMETER,
    /* .name */ "be non null",
    /* .destroy */ &destroy_static_constraint,
    /* .compare */ &compare_do_not_want_value,
    /* .test */ &test_do_not_want_value,
    /* .stored_value */ 0,
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0
};

Constraint static_is_null_constraint = {
    /* .type */ PARAMETER,
    /* .name */ "be null",
    /* .destroy */ &destroy_static_constraint,
    /* .compare */ &compare_want_value,
    /* .test */ &test_want_value,
    /* .stored_value */ 0,
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0
};

Constraint static_is_false_constraint = { 
    /* .type */ PARAMETER,
    /* .name */ "be false",
    /* .destroy */ &destroy_static_constraint,
    /* .compare */ &compare_want_value,
    /* .test */ &test_want_value,
    /* .stored_value */ 0,
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0
};

Constraint static_is_true_constraint = { 
    /* .type */ PARAMETER,
    /* .name */ "be true",
    /* .destroy */ &destroy_static_constraint,
    /* .compare */ &compare_do_not_want_value,
    /* .test */ &test_do_not_want_value,
    /* .stored_value */ 0,
    /* .parameter_name */ NULL,
    /* .size_of_stored_value */ 0
};

Constraint *is_non_null = &static_is_non_null_constraint;
Constraint *is_null = &static_is_null_constraint;
Constraint *is_false = &static_is_false_constraint;
Constraint *is_true = &static_is_true_constraint;


Constraint *is_equal_to_string(const char *value_to_match)
{
    return create_equal_to_string_constraint(value_to_match);
}

Constraint *is_not_equal_to_string(const char *value_to_match)
{
    return create_not_equal_to_string_constraint(value_to_match);
}

Constraint *contains_string(const char *value_to_match)
{
    return create_contains_string_constraint(value_to_match);
}

Constraint *does_not_contain_string(const char *value_to_match)
{
    return create_does_not_contain_string_constraint(value_to_match);
}

Constraint *is_equal_to_double(double value_to_match)
{
    return create_equal_to_double_constraint(value_to_match);
}

Constraint *is_not_equal_to_double(double value_to_match)
{
    return create_not_equal_to_double_constraint(value_to_match);
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
