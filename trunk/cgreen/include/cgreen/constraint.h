#ifndef _CONSTRAINT_HEADER_
#define _CONSTRAINT_HEADER_

#include <inttypes.h>
#include "reporter.h"

#define want(parameter, x) _want(#parameter, (intptr_t)x)
#define want_string(parameter, x) _want_string(#parameter, x)
#define compare_constraint(c, x) (*c->compare)(c, (intptr_t)x)

typedef struct _Constraint Constraint;
struct _Constraint {
    const char *parameter;
    void (*destroy)(Constraint *);
    int (*compare)(Constraint *, intptr_t);
    void (*test)(Constraint *, const char *, intptr_t, const char *, int, TestReporter *);
    intptr_t expected;
};

void destroy_constraint(void *constraint);
int is_constraint_parameter(Constraint *constraint, const char *label);
void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
Constraint *_want(const char *parameter, intptr_t expected);
Constraint *_want_string(const char *parameter, char *expected);

#endif
