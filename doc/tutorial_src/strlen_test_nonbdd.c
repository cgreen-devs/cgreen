#include <cgreen/cgreen.h>
#include <string.h>

Ensure(strlen_of_hello_is_five) {
    int length = strlen("Hello\0");
    assert_that(length, is_equal_to(5));
}

Ensure(strlen_of_empty_string_is_zero) {
    int length = strlen("\0");
    assert_that(length, is_equal_to(0));
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
