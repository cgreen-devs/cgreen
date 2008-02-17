#include "../cgreen.h"
#include "../breadcrumb.h"
#include <stdlib.h>

Ensure a_test_that_passes() {
    assert_true(1);
}

Ensure another_test_that_passes() {
    assert_true(1);
}

TestSuite *confuse_the_collector(TestSuite *suite, const char *ignored) {
    return suite;
}

TestSuite *collector_tests() {
    TestSuite *suite = create_test_suite();
    add_tests(confuse_the_collector(suite, "\"("));
    return suite;
}
