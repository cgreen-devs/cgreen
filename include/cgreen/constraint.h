#ifndef _CONSTRAINT_HEADER_
#define _CONSTRAINT_HEADER_

#include <inttypes.h>

#define is(parameter, x) _is(#parameter, x)
#define string_is(parameter, x) _string_is(#parameter, x)

typedef struct _CgreenConstraint CgreenConstraint;

CgreenConstraint *_is(const char *parameter, intptr_t comparison);
CgreenConstraint *_string_is(const char *parameter, char *comparison);

#endif
