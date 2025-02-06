#include <cgreen/cgreen.h>

// This funciton exemplifies infrastructure common between multiple suits 
int common_func_btw_SUTs()
{
    return 0;
}

// Suite A and its unit test
Describe(SUT_A);
BeforeEach(SUT_A) {}
AfterEach(SUT_A) {}

Ensure(SUT_A, ut_a) {
    int num = common_func_btw_SUTs();
    assert_that(num, is_equal_to(0));
}


// Suite B and its unit test
Describe(SUT_B);
BeforeEach(SUT_B) {}
AfterEach(SUT_B) {}

Ensure(SUT_B, ut_b) {
    int num = common_func_btw_SUTs();
    assert_that(num, is_equal_to(0));
}

int main(int argc, char **argv) {

    TestSuite *suiteA = create_test_suite();
    TestSuite *suiteB = create_test_suite();
    add_test_with_context(suiteA, SUT_A, ut_a);
    add_test_with_context(suiteB, SUT_B, ut_b);

    run_test_suite(suiteA, create_text_reporter());
    run_test_suite(suiteB, create_text_reporter());

    return 0;
}
