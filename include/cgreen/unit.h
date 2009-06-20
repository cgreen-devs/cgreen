#ifndef UNIT_HEADER
#define UNIT_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/reporter.h>
#include <cgreen/mocks.h>

/**
 * @defgroup unit_tests Unit Test Functions
 *
 * @brief Functions to create a Test Suite and adding tests.
 */

/**
 * @addtogroup unit_tests
 * @{
 */

/**
 * @brief Creates a new test suite.
 *
 * @return A newly allocated test suite, NULL on error.
 *
 * @see destroy_test_suite()
 */
#define create_test_suite() create_named_test_suite(__func__)
#define add_test(suite, test) add_test_(suite, (char *) #test, &test)
#define add_tests(suite, ...) add_tests_(suite, #__VA_ARGS__, (CgreenTest *)__VA_ARGS__ +0)
#define add_suite(owner, suite) add_suite_(owner, (char *) #suite, suite)
#define setup(suite, function) setup_(suite, &function)
#define teardown(suite, function) teardown_(suite, &function)

#define Ensure static void
typedef struct TestSuite_ TestSuite;
typedef void CgreenTest();

/**
 * @brief Create a new test suite with a special name.
 *
 * @param  name         The name of the test suite.
 *
 * @return A newly allocated test suite, NULL on error.
 *
 * @see destroy_test_suite()
 */
TestSuite *create_named_test_suite(const char *name);

/**
 * @brief Free the memory of a test suite.
 *
 * @param  suite        The test suite to free.
 */
void destroy_test_suite(TestSuite *suite);
void add_test_(TestSuite *suite, char *name, CgreenTest *test);
void add_tests_(TestSuite *suite, const char *names, ...);
void add_suite_(TestSuite *owner, char *name, TestSuite *suite);
void setup_(TestSuite *suite, void (*set_up)());
void teardown_(TestSuite *suite, void (*tear_down)());
void die_in(unsigned int seconds);
int run_test_suite(TestSuite *suite, TestReporter *reporter);
int run_single_test(TestSuite *suite, char *test, TestReporter *reporter);

/**
 * @}
 */

#ifdef __cplusplus
    }
#endif

#endif
