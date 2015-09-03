#include <cgreen/cgreen.h>
#include <string.h>

Ensure(strlen_of_hello_is_five) {
    assert_that(strlen("Hello"), is_equal_to(5));
}

Ensure(strlen_of_empty_string_is_zero) {
    assert_that(strlen("\0"), is_equal_to(0));
}

TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, strlen_of_hello_is_five);
    add_test(suite, strlen_of_empty_string_is_zero);
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(our_tests(), create_text_reporter());
}
