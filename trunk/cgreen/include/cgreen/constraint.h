#ifndef CONSTRAINT_HEADER
#define CONSTRAINT_HEADER

#include <cgreen/reporter.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

typedef enum {
    PARAMETER,
    RETURN_VALUE,
    SET_PARAMETER,
    CALL
} ConstraintType;

typedef struct Constraint_ Constraint;
struct Constraint_ {
    ConstraintType type;
    const char *name;
    void (*destroy)(Constraint *);
    int (*compare)(Constraint *, intptr_t);
    void(*test)(Constraint *, const char *, intptr_t, const char *, int, TestReporter *);
    intptr_t expected_value;
    const char *expected_value_name;

    /* for PARAMETER constraints */
    const char *parameter_name;
    size_t size_of_stored_value;
};

Constraint *create_constraint();
Constraint *create_parameter_constraint_for(const char *parameter_name);

void destroy_empty_constraint(Constraint *constraint);
void destroy_static_constraint(Constraint *constraint);
void destroy_double_constraint(Constraint *constraint);
void destroy_constraint(Constraint *);

int compare_want_value(Constraint *constraint, intptr_t actual);
int compare_do_not_want_value(Constraint *constraint, intptr_t actual);
void test_want_value(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
void test_do_not_want_value(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

bool constraint_is_for_parameter(const Constraint *, const char *);
void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

Constraint *create_equal_to_value_constraint(intptr_t expected_value, const char *expected_value_name);
Constraint *create_not_equal_to_value_constraint(intptr_t expected_value, const char *expected_value_name);
Constraint *create_greater_than_value_constraint(intptr_t expected_value, const char *expected_value_name);
Constraint *create_less_than_value_constraint(intptr_t expected_value, const char *expected_value_name);
Constraint *create_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare, const char *compared_pointer_name);
Constraint *create_not_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare, const char *compared_pointer_name);
Constraint *create_equal_to_string_constraint(const char* expected_value, const char *expected_value_name);
Constraint *create_not_equal_to_string_constraint(const char* expected_value, const char *expected_value_name);
Constraint *create_contains_string_constraint(const char* expected_value, const char *expected_value_name);
Constraint *create_does_not_contain_string_constraint(const char* expected_value, const char *expected_value_name);
Constraint *create_equal_to_double_constraint(double expected_value, const char *expected_value_name);
Constraint *create_not_equal_to_double_constraint(double expected_value, const char *expected_value_name);
Constraint *create_return_value_constraint(intptr_t value_to_return);
Constraint *create_set_parameter_value_constraint(const char *parameter_name, intptr_t value_to_set, size_t size_to_set);


#ifdef __cplusplus
    }
}
#endif

#endif
