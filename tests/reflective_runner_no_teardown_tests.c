#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static int counter = 0;

/* this suite exercises a former crash bug in the reflective runner by not having a teardown */
Describe(ReflectiveNoTeardownTest);

BeforeEach(ReflectiveNoTeardownTest) {
    counter += 5;
}

AfterEach(ReflectiveNoTeardownTest) {}

Ensure(ReflectiveNoTeardownTest, before_called_implicitly_before_each_test) {
    assert_that(counter, is_equal_to(5));
}

Ensure(ReflectiveNoTeardownTest, second_test_unaffected_by_first) {
    assert_that(counter, is_equal_to(5));
}

TestSuite *reflective_no_teardown_tests(void) {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, ReflectiveNoTeardownTest, before_called_implicitly_before_each_test);
    add_test_with_context(suite, ReflectiveNoTeardownTest, second_test_unaffected_by_first);

    return suite;
}
