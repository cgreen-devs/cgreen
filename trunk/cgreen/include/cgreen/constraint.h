#ifndef CONSTRAINT_HEADER
#define CONSTRAINT_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/reporter.h>
#include <stdbool.h>
#include <stdint.h>

#define compare_constraint(c, x) (*c->compare)(c, (intptr_t)x)

typedef enum {
    PARAMETER,
    RETURN_VALUE,
    CALL
} ConstraintType;

typedef struct Constraint_ Constraint;
struct Constraint_ {
    ConstraintType type;
    const char *name;
    void (*destroy)(Constraint *);
    int (*compare)(Constraint *, intptr_t);
    void(*test)(Constraint *, const char *, intptr_t, const char *, int, TestReporter *);

    /* for PARAMETER constraints */
    const char *parameter_name;

    /* for RETURN_VALUE constraints */
    intptr_t stored_value;
};

Constraint *create_constraint();
Constraint *create_parameter_constraint_for(const char *parameter_name);

void destroy_empty_constraint(Constraint *constraint);
void destroy_static_constraint(Constraint *constraint);
void destroy_double_constraint(Constraint *constraint);
void destroy_constraint(Constraint *);

int compare_want(Constraint *constraint, intptr_t actual);
int compare_do_not_want(Constraint *constraint, intptr_t actual);
void test_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
void test_do_not_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

bool constraint_is_for_parameter(const Constraint *, const char *);
void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

Constraint *create_equal_to_intptr_constraint(intptr_t value_to_match);
Constraint *create_not_equal_to_intptr_constraint(intptr_t value_to_match);
Constraint *create_equal_to_string_constraint(const char* value_to_match);
Constraint *create_not_equal_to_string_constraint(const char* value_to_match);
Constraint *create_contains_string_constraint(const char* value_to_match);
Constraint *create_does_not_contain_string_constraint(const char* value_to_match);
Constraint *create_equal_to_double_constraint(double value_to_match);
Constraint *create_not_equal_to_double_constraint(double value_to_match);
Constraint *create_return_value_constraint(intptr_t value_to_return);


#ifdef __cplusplus
    }
#endif

#endif
