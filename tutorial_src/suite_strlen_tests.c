#include <cgreen/cgreen.h>
#include <string.h>

Describe(Strlen);
BeforeEach(Strlen) {}
AfterEach(Strlen) {}

Ensure(Strlen, returns_five_for_hello) {
    assert_that(strlen("Hiya"), is_equal_to(5));
}

Ensure(Strlen, returns_zero_for_empty_string) {
    assert_equal(strlen("\0"), 0);
}

TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Strlen, returns_five_for_hello);
    add_test_with_context(suite, Strlen, returns_zero_for_empty_string);
    return suite;
}

