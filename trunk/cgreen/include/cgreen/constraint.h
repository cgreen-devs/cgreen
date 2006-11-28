#ifndef _CONSTRAINT_HEADER_
#define _CONSTRAINT_HEADER_

#include <inttypes.h>

#define exactly(parameter, x) _exactly(#parameter, x)

typedef struct _CgreenConstraint CgreenConstraint;

CgreenConstraint *_exactly(const char *parameter, intptr_t comparison);

#endif
