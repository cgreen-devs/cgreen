#include "constraint.h"
#include <inttypes.h>

struct _CgreenConstraint {
    void *compare;
    void (*destroy)(CgreenConstraint *);
    intptr_t target;
};

CgreenConstraint *_exactly(const char *parameter, intptr_t comparison) {
}
