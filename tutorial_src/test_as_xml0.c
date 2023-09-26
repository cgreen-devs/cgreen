#include <cgreen/cgreen.h>

Describe(XML_reporter);
BeforeEach(XML_reporter) {}
AfterEach(XML_reporter) {}

Ensure(XML_reporter, reports_a_test_that_passes) {
    assert_that(1 == 1);
}

Ensure(XML_reporter, reports_a_test_that_fails) {
    fail_test("A failure");
}

TestSuite *create_test_group() {
    TestSuite *suite = create_named_test_suite("A Group");
    add_test_with_context(suite, XML_reporter, reports_a_test_that_passes);
    add_test_with_context(suite, XML_reporter, reports_a_test_that_fails);
    return suite;
}

int main(int argc, char **argv) {
    TestSuite *suite = create_named_test_suite("Top Level");
    add_suite(suite, create_test_group());
    return run_test_suite(suite, create_text_reporter());
}
