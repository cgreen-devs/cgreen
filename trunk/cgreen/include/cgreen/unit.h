#ifndef UNIT_HEADER
#define UNIT_HEADER

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#include "internal/unit_implementation.h"

/* BDD style: Describe the Subject Under Test, or context, by name */
#define Describe(subject) DescribeImplementation(subject)

/* BDD style: Run this before any tests for that SUT or in that context */
#define BeforeEach(subject) BeforeEachImplementation(subject)

/* BDD style: Run this after any tests for that SUT or in that context */
#define AfterEach(subject) AfterEachImplementation(subject)

/* NOTE if you use BDD style all three of the above are required */
/* Then you must also use the BDD style Ensure(subject, test) */

/* TDD Style: Ensure(testname) {implementation} */
/* BDD Style: Ensure(subject, testname) {implementation} */
#define Ensure(...) Ensure_NARG(__VA_ARGS__)(__VA_ARGS__)

#ifdef __cplusplus
    }
}
#endif

#endif
