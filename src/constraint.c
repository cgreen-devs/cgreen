#include "constraint.h"
#include <inttypes.h>

struct _CgreenConstraint {
    void *compare;
    void (*destroy)(CgreenConstraint *);
    intptr_t target;
};

CgreenConstraint *_is(const char *parameter, intptr_t comparison) {
}

CgreenConstraint *_string_is(const char *parameter, char *comparison) {
}
