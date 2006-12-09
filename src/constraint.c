#include "constraint.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

void destroy_empty_constraint(Constraint *constraint);
int compare_want(Constraint *constraint, intptr_t comparison);
int compare_want_string(Constraint *constraint, intptr_t comparison);
Constraint *create_constraint(const char *parameter);

void destroy_constraint(void *abstract) {
    Constraint *constraint = (Constraint *)abstract;
    (*constraint->destroy)(constraint);
}

int is_constraint_parameter(Constraint *constraint, const char *parameter) {
    return strcmp(constraint->parameter, parameter) == 0;
}

Constraint *_want(const char *parameter, intptr_t expected) {
    Constraint *constraint = create_constraint(parameter);
    constraint->parameter = parameter;
    constraint->compare = &compare_want;
    constraint->expected = expected;
    return constraint;
}

Constraint *_want_string(const char *parameter, char *expected) {
    Constraint *constraint = create_constraint(parameter);
    constraint->compare = &compare_want_string;
    constraint->expected = (intptr_t)expected;
    return constraint;
}

void destroy_empty_constraint(Constraint *constraint) {
    free(constraint);
}

int compare_want(Constraint *constraint, intptr_t comparison) {
    return (constraint->expected == comparison);
}

int compare_want_string(Constraint *constraint, intptr_t comparison) {
    if (((void *)comparison == NULL) || ((void *)constraint->expected == NULL)) {
        return ((void *)constraint->expected == (void *)comparison);
    }
    return (strcmp((const char *)constraint->expected, (const char *)comparison) == 0);
}

Constraint *create_constraint(const char *parameter) {
    Constraint *constraint = (Constraint *)malloc(sizeof(Constraint));
    constraint->parameter = parameter;
    constraint->destroy = &destroy_empty_constraint;
    return constraint;
}
