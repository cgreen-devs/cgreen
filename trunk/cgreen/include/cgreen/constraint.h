#ifndef CONSTRAINT_HEADER
#define CONSTRAINT_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/reporter.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#define compare_constraint(c, x) (*c->compare)(c, (intptr_t)x)

typedef int (*Matcher)(intptr_t);

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

Constraint *create_parameter_constraint_for(const char *parameter_name);
void destroy_constraint(void *);
bool constraint_is_for_parameter(const Constraint *, const char *);
void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

#define is_equal_to(x) create_equal_to_intptr_constraint(x)
#define is_not_equal_to(x) create_not_equal_to_intptr_constraint(x)
#define is_equal_to_string(x) create_equal_to_string_constraint(x)
#define is_not_equal_to_string(x) create_not_equal_to_string_constraint(x)
#define contains_string(x) create_contains_string_constraint(x)
#define does_not_contain_string(x) create_does_not_contain_string_constraint(x)
#define is_equal_to_double(x) create_equal_to_double_constraint(x)
#define is_not_equal_to_double(x) create_not_equal_to_double_constraint(x)

#define is_non_null create_not_equal_to_intptr_constraint(0)
#define is_null create_equal_to_intptr_constraint(0)
#define is_false create_equal_to_intptr_constraint(0)
#define is_true create_not_equal_to_intptr_constraint(0)

#define will_return(x) create_return_value_constraint((intptr_t)x)

Constraint *create_equal_to_intptr_constraint(intptr_t value_to_match);
Constraint *create_not_equal_to_intptr_constraint(intptr_t value_to_match);
Constraint *create_equal_to_string_constraint(char* value_to_match);
Constraint *create_not_equal_to_string_constraint(char* value_to_match);
Constraint *create_contains_string_constraint(char* value_to_match);
Constraint *create_does_not_contain_string_constraint(char* value_to_match);
Constraint *create_equal_to_double_constraint(double value_to_match);
Constraint *create_not_equal_to_double_constraint(double value_to_match);
Constraint *create_return_value_constraint(intptr_t value_to_return);

#ifdef __cplusplus
    }
#endif

#endif
