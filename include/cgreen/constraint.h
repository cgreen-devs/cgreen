#ifndef CONSTRAINT_HEADER
#define CONSTRAINT_HEADER

#include <cgreen/reporter.h>
#include <cgreen/cgreen_value.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

typedef enum {
    VALUE_COMPARER,
    CONTENT_COMPARER,
    STRING_COMPARER,
    DOUBLE_COMPARER,
    RETURN_VALUE,
    CONTENT_SETTER,
    CALL,
    CALL_COUNTER
} ConstraintType;

typedef struct CgreenConstraint_ CgreenConstraint;
struct CgreenConstraint_ {
    ConstraintType type;
    const char *name;
    void (*destroy)(CgreenConstraint *);
    bool (*compare)(CgreenConstraint *, CgreenValue);
    void (*execute)(CgreenConstraint *, const char *, CgreenValue, const char *, int, TestReporter *);
    char *(*failure_message)(CgreenConstraint *, const char *, intptr_t);
    const char *actual_value_message;
    const char *expected_value_message;
    CgreenValue expected_value;
    const char *expected_value_name;

    /* for PARAMETER constraints */
    const char *parameter_name;
    size_t size_of_expected_value;

    /* Side Effect parameters */
    void (*side_effect_callback)(void *);
    void *side_effect_data;
};

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

CgreenConstraint *create_constraint(void);
CgreenConstraint *create_parameter_constraint_for(const char *parameter_name);

bool compare_want_value(CgreenConstraint *constraint, CgreenValue actual);
bool compare_do_not_want_value(CgreenConstraint *constraint, CgreenValue actual);
void test_want(CgreenConstraint *constraint, const char *function, CgreenValue actual, const char *test_file, int test_line, TestReporter *reporter);

void test_constraint(CgreenConstraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

CgreenConstraint *create_equal_to_value_constraint(intptr_t expected_value, const char *expected_value_name);
CgreenConstraint *create_equal_to_hexvalue_constraint(intptr_t expected_value, const char *expected_value_name);
CgreenConstraint *create_not_equal_to_value_constraint(intptr_t expected_value, const char *expected_value_name);
CgreenConstraint *create_greater_than_value_constraint(intptr_t expected_value, const char *expected_value_name);
CgreenConstraint *create_less_than_value_constraint(intptr_t expected_value, const char *expected_value_name);
CgreenConstraint *create_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare, const char *compared_pointer_name);
CgreenConstraint *create_not_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare, const char *compared_pointer_name);
CgreenConstraint *create_equal_to_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_not_equal_to_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_contains_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_does_not_contain_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_begins_with_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_does_not_begin_with_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_ends_with_string_constraint(const char* expected_value, const char *expected_value_name);
CgreenConstraint *create_does_not_end_with_string_constraint(const char* expected_value, const char *expected_value_name);

CgreenConstraint *create_equal_to_double_constraint(double expected_value, const char *expected_value_name);
CgreenConstraint *create_not_equal_to_double_constraint(double expected_value, const char *expected_value_name);
CgreenConstraint *create_less_than_double_constraint(double expected_value, const char *expected_value_name);
CgreenConstraint *create_greater_than_double_constraint(double expected_value, const char *expected_value_name);
CgreenConstraint *create_return_value_constraint(intptr_t value_to_return);
CgreenConstraint *create_return_double_value_constraint(double value_to_return);
CgreenConstraint *create_set_parameter_value_constraint(const char *parameter_name, intptr_t value_to_set, size_t size_to_set);
CgreenConstraint *create_with_side_effect_constraint(void (*callback)(void *), void *data);

/* Utility: */
int get_significant_figures(void);
void significant_figures_for_assert_double_are(int figures);

#ifdef __cplusplus
    }
}
#endif

#endif
