#ifndef _CONSTRAINT_HEADER_
#define _CONSTRAINT_HEADER_

#include <inttypes.h>

#define is(parameter, x) _is(#parameter, (intptr_t)x)
#define is_string(parameter, x) _is_string(#parameter, x)
#define compare_constraint(c, x) _compare_constraint(c, (intptr_t)x)

typedef struct _CgreenConstraint CgreenConstraint;
struct _CgreenConstraint {
    const char *parameter;
    void (*destroy)(CgreenConstraint *);
    int (*compare)(CgreenConstraint *, intptr_t);
    intptr_t expected;
};

void destroy_constraint(CgreenConstraint *constraint);
int _compare_constraint(CgreenConstraint *constraint, intptr_t comparison);
CgreenConstraint *_is(const char *parameter, intptr_t expected);
CgreenConstraint *_is_string(const char *parameter, char *expected);

#endif
