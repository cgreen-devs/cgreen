#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static int counter = 0;

/* this suite exercises a former crash bug in the reflective runner by not having a teardown */
Describe(ReflectiveNoTeardownTests, with_setup);

void setup() {
    counter += 5;
}

Ensure(ReflectiveNoTeardownTests, before_called_implicitly_before_each_test) {
    assert_that(counter, is_equal_to(5));
}

Ensure(ReflectiveNoTeardownTests, second_test_unaffected_by_first) {
    assert_that(counter, is_equal_to(5));
}

TestSuite *reflective_no_teardown_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, ReflectiveNoTeardownTests, before_called_implicitly_before_each_test);
    add_test_with_context(suite, ReflectiveNoTeardownTests, second_test_unaffected_by_first);

    return suite;
}
