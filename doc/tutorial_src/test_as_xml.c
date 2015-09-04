#include <cgreen/cgreen.h>
#include <stdbool.h>
#include "xml_reporter.h"

Ensure(a_test_that_passes) {
    assert_that(1 == 1);
}

Ensure(a_test_that_fails) {
    fail("A failure");
}

TestSuite *create_test_group() {
    TestSuite *suite = create_named_test_suite("A Group");
    add_test(suite, a_test_that_passes);
    add_test(suite, a_test_that_fails);
    return suite;
}

int main(int argc, char **argv) {
    TestSuite *suite = create_named_test_suite("Top Level");
    add_suite(suite, create_test_group());
    return run_test_suite(suite, create_xml_reporter());
}
