#include <cgreen/cgreen.h>
#include <string.h>

Ensure(strlen_of_hello_should_be_five) {
    assert_equal(strlen("Hello"), 5);
}

TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, strlen_of_hello_should_be_five);
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(our_tests(), create_text_reporter());
}
