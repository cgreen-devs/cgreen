#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static int counter = 0;

Describe(ReflectiveRunner, with_setup, withTeardown);

void setup() {
    counter += 5;
}

void teardown() {
    counter += 1;
}

Ensure(ReflectiveRunner, outputs_exceptional_error) {
//	char *foo = NULL;
//	*foo = '1';
}

Ensure(ReflectiveRunner, calls_setup_before_each_test) {
    assert_that(counter, is_equal_to(5));
}

Ensure(ReflectiveRunner, running_first_test_does_not_affect_second_test) {
    assert_that(counter, is_equal_to(5));
}

TestSuite *reflective_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, ReflectiveRunner, calls_setup_before_each_test);
    add_test_with_context(suite, ReflectiveRunner, running_first_test_does_not_affect_second_test);

    return suite;
}
