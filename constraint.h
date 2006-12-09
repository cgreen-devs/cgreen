#ifndef _CONSTRAINT_HEADER_
#define _CONSTRAINT_HEADER_

#include <inttypes.h>
#include "reporter.h"

#define want(parameter, x) _want(#parameter, (intptr_t)x)
#define want_string(parameter, x) _want_string(#parameter, x)
#define compare_constraint(c, x) (*c->compare)(c, (intptr_t)x)
#define fail_constraint(c, x, reporter) (*c->fail)(c, (intptr_t)x, reporter)

typedef struct _Constraint Constraint;
struct _Constraint {
    const char *parameter;
    void (*destroy)(Constraint *);
    int (*compare)(Constraint *, intptr_t);
    void (*fail)(Constraint *, intptr_t, TestReporter *reporter);
    intptr_t expected;
};

void destroy_constraint(void *constraint);
int is_constraint_parameter(Constraint *constraint, const char *label);
Constraint *_want(const char *parameter, intptr_t expected);
Constraint *_want_string(const char *parameter, char *expected);

#endif
