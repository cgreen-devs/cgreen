#include "constraint.h"
#include <inttypes.h>
#include <stdlib.h>

void destroy_empty_constraint(CgreenConstraint *constraint);
int compare_is(CgreenConstraint *constraint, intptr_t comparison);
int compare_is_string(CgreenConstraint *constraint, intptr_t comparison);
CgreenConstraint *create_constraint(const char *parameter);

void destroy_constraint(CgreenConstraint *constraint) {
    (*constraint->destroy)(constraint);
}

int _compare_constraint(CgreenConstraint *constraint, intptr_t comparison) {
    (*constraint->compare)(constraint, comparison);
}

CgreenConstraint *_is(const char *parameter, intptr_t expected) {
    CgreenConstraint *constraint = create_constraint(parameter);
    constraint->compare = &compare_is;
    constraint->expected = expected;
    return constraint;
}

CgreenConstraint *_is_string(const char *parameter, char *expected) {
    CgreenConstraint *constraint = create_constraint(parameter);
    constraint->compare = &compare_is_string;
    constraint->expected = (intptr_t)expected;
    return constraint;
}

void destroy_empty_constraint(CgreenConstraint *constraint) {
    free(constraint);
}

int compare_is(CgreenConstraint *constraint, intptr_t comparison) {
    return (constraint->expected == comparison);
}

int compare_is_string(CgreenConstraint *constraint, intptr_t comparison) {
    if ((const char *)comparison == NULL) {
        return ((const char *)constraint->expected == (const char *)comparison);
    }
    return (strcmp((const char *)constraint->expected, (const char *)comparison) == 0);
}

CgreenConstraint *create_constraint(const char *parameter) {
    CgreenConstraint *constraint = (CgreenConstraint *)malloc(sizeof(CgreenConstraint));
    constraint->parameter = parameter;
    constraint->destroy = &destroy_empty_constraint;
    return constraint;
}
