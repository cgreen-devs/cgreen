#include "cgreen/cgreen.h"
#include <string.h>

static void strlen_of_hello_should_be_five() {
    int length = strlen("Hello\0");
    assert_equal(length, 5, "Should be 5, but was %d", length);
}

static void strlen_of_empty_string_should_be_zero() {
    int length = strlen("\0");
    assert_equal(length, 0, "Should be 0, but was %d", length);
}

TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, strlen_of_hello_should_be_five);
    add_test(suite, strlen_of_empty_string_should_be_zero);
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(our_tests(), create_text_reporter());
}
