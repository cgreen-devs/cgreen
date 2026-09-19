#include <cgreen/cgreen.h>
#include <string.h>

Describe(Strlen);                                                 // <1>
BeforeEach(Strlen) {}                                             // <2>
AfterEach(Strlen) {}                                              // <3>

Ensure(Strlen, returns_five_for_hello) {                          // <4>
    assert_that(strlen("Hello"), is_equal_to(5));
}

TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Strlen, returns_five_for_hello); // <5>
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(our_tests(), create_text_reporter());
}
