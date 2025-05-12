#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

// This funciton exemplifies infrastructure common between multiple suits 
int common_func_btw_SUTs()
{
    return 0;
}

// Suite A and its unit test
Describe(SuiteA);
BeforeEach(SuiteA) {}
AfterEach(SuiteA) {}

Ensure(SuiteA, ut_a) {
    int num = common_func_btw_SUTs();
    assert_that(num, is_equal_to(0));
}

// Suite B and its unit test
Describe(SuiteB);
BeforeEach(SuiteB) {}
AfterEach(SuiteB) {}

Ensure(SuiteB, ut_b) {
    int num = common_func_btw_SUTs();
    assert_that(num, is_equal_to(0));
}

TestSuite *test_suite_a(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, SuiteA, ut_a);
    run_test_suite(suite, create_text_reporter());
    return suite;
}

TestSuite *test_suite_b(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, SuiteB, ut_b);
    run_test_suite(suite, create_text_reporter());
    return suite;
}
