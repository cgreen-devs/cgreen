#include <cgreen/cgreen.h>
#include <string.h>
                                                               // <1>
Ensure(strlen_returns_five_for_hello) {                        // <2>
    assert_that(strlen("Hello"), is_equal_to(5));
}

TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, strlen_returns_five_for_hello);            // <3>
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(our_tests(), create_text_reporter());
}
