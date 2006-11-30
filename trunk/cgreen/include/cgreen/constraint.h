#ifndef _CONSTRAINT_HEADER_
#define _CONSTRAINT_HEADER_

#include <inttypes.h>

#define is(parameter, x) _is(#parameter, x)
#define string_is(parameter, x) _string_is(#parameter, x)

typedef struct _CgreenConstraint CgreenConstraint;
struct _CgreenConstraint {
    const char *parameter;
    void (*destroy)(CgreenConstraint *);
    int (*compare)(intptr_t);
    intptr_t expected;
};

CgreenConstraint *_is(const char *parameter, intptr_t expected);
void is_constraint_destroy(CgreenConstraint *constraint);
int is_constraint_compare(intptr_t comparison);
CgreenConstraint *_string_is(const char *parameter, char *expected);

#endif
