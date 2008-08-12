#ifndef CONSTRAINT_HEADER
#define CONSTRAINT_HEADER

#include <inttypes.h>
#include <cgreen/reporter.h>

#define want(parameter, x) want_(#parameter, (intptr_t)x)
#define want_string(parameter, x) want_string_(#parameter, x)
#define want_double(parameter, x) want_double_(#parameter, box_double(x))
#define compare_constraint(c, x) (*c->compare)(c, (intptr_t)x)

typedef union {
    double d;
} BoxedDouble;

typedef struct Constraint_ Constraint;
struct Constraint_ {
    const char *parameter;
    void (*destroy)(Constraint *);
    int (*compare)(Constraint *, intptr_t);
    void (*test)(Constraint *, const char *, intptr_t, const char *, int, TestReporter *);
    intptr_t expected;
};

void destroy_constraint(void *constraint);
int is_constraint_parameter(Constraint *constraint, const char *label);
void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
Constraint *want_(const char *parameter, intptr_t expected);
Constraint *want_string_(const char *parameter, char *expected);
Constraint *want_double_(const char *parameter, intptr_t expected);
intptr_t box_double(double d);

#endif
