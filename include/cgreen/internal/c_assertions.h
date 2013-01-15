#ifndef C_ASSERTIONS_HEADER
#define C_ASSERTIONS_HEADER

#include <cgreen/constraint.h>
#include <inttypes.h>

#ifdef __cplusplus
#error "Should only be included when not compiling in C++ mode. Include cpp_assertions.h instead."
#endif

#define assert_that_constraint(actual, constraint) assert_that_(__FILE__, __LINE__, #actual, (intptr_t)actual, constraint)

// this isn't declared in assertions.h because you can't have overloads for an extern "C"-declared function, so it seems
void assert_that_(const char *file, int line, const char *actual_string, intptr_t actual, Constraint *constraint);

#endif
