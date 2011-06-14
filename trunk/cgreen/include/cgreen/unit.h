#ifndef UNIT_HEADER
#define UNIT_HEADER

#include <cgreen/reporter.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

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


#define spec_name(contextName, spec) CgreenSpec_##contextName##_##spec

#define create_test_suite() create_named_test_suite(__func__)
#define add_test(suite, test) add_test_(suite, #test, &spec_name(default, test))
#define add_test_with_context(suite, context, test) add_test_(suite, #test, &spec_name(context, test))
#define add_tests(suite, ...) add_tests_(suite, #__VA_ARGS__, (CgreenTest *)__VA_ARGS__ +0)
#define add_suite(owner, suite) add_suite_(owner, #suite, suite)

typedef struct {
    const char* name;
    const char* filename;
    void (*setup)(void);
    void (*teardown)(void);
} CgreenContext;

typedef struct {
    CgreenContext* context;
    const char* name;
    void(*run)(void);
} CgreenTest;

extern CgreenContext defaultContext;

#define Before() static void Before(void)
#define After() static void After(void)

Before();
After();

#define Describe(x)\
static CgreenContext contextFor##x = { #x, __FILE__, &Before, &After };

#define Ensure_NARG(...) \
         Ensure_NARG_(__VA_ARGS__,Ensure_RSEQ_N())

#define Ensure_NARG_(...) \
         Ensure_ARG_N(__VA_ARGS__)

#define Ensure_ARG_N( \
          _1,_2,N,...) N

#define Ensure_RSEQ_N() \
        EnsureWithContextAndSpecificationName, EnsureWithSpecificationName, ENSURE_REQUIRES_NAME

#define Ensure(...) Ensure_NARG(__VA_ARGS__)(__VA_ARGS__)

#define EnsureWithContextAndSpecificationName(contextName, spec, ...) \
	static void contextName##_##spec (void);\
	CgreenTest spec_name(contextName, spec) = { &contextFor##contextName, #spec, &contextName##_##spec };\
	static void contextName##_##spec ()

#define EnsureWithSpecificationName(spec, ...) \
	static void spec (void);\
	CgreenTest spec_name(default, spec) = { &defaultContext, #spec, &spec };\
	static void spec ()

////////////////////////////////////////////////////////////////////////////////

typedef struct TestSuite_ TestSuite;

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
void add_test_(TestSuite *suite, const char *name, CgreenTest *test);
void add_tests_(TestSuite *suite, const char *names, ...);
void add_suite_(TestSuite *owner, const char *name, TestSuite *suite);
void setup(TestSuite *suite, void (*set_up)());
void teardown(TestSuite *suite, void (*tear_down)());
void die_in(unsigned int seconds);
int run_test_suite(TestSuite *suite, TestReporter *reporter);
int run_single_test(TestSuite *suite, const char *test, TestReporter *reporter);
int count_tests(TestSuite *suite);

/**
 * @}
 */

#ifdef __cplusplus
    }
}
#endif

#endif
