#ifndef C_ASSERTIONS_HEADER
#define C_ASSERTIONS_HEADER

#include <cgreen/constraint.h>
#include <inttypes.h>

#ifndef __cplusplus
#define assert_that_constraint(actual, constraint) assert_core_(__FILE__, __LINE__, #actual, (intptr_t)actual, constraint)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void assert_core_(const char *file, int line, const char *actual_string, intptr_t actual, Constraint *constraint);

#ifdef __cplusplus
}
#endif

#endif
