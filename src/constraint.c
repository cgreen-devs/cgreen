#include "constraint.h"
#include <inttypes.h>
#include <stdlib.h>

CgreenConstraint *_is(const char *parameter, intptr_t expected) {
    CgreenConstraint *constraint = (CgreenConstraint *)malloc(sizeof(CgreenConstraint));
    constraint->parameter = parameter;
    constraint->destroy = &is_constraint_destroy;
    constraint->compare = &is_constraint_compare;
    constraint->expected = expected;
    return constraint;
}

void is_constraint_destroy(CgreenConstraint *constraint) {
    free(constraint);
}

int is_constraint_compare(intptr_t comparison) {
}

CgreenConstraint *_string_is(const char *parameter, char *expected) {
}
