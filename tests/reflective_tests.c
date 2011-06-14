#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static int counter = 0;

Describe(ReflectiveTests);

Before() {
    counter += 5;
}

After() {
    counter += 1;
}

Ensure(ReflectiveTests, before_called_implicitly_before_each_test) {
    assert_equal(counter, 5);
}

Ensure(ReflectiveTests, second_test_unaffected_by_first) {
    assert_equal(counter, 5);
}

TestSuite *reflective_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, ReflectiveTests, before_called_implicitly_before_each_test);
    add_test_with_context(suite, ReflectiveTests, second_test_unaffected_by_first);

    return suite;
}
