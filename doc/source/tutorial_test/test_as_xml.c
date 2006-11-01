#include "cgreen/cgreen.h"
#include "xml_reporter.h"

void a_test_that_passes() {
    assert_true(1);
}

void a_test_that_fails() {
    assert_true_with_message(0, "A failure");
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
