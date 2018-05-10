#include <cgreen/cgreen.h>
#include <string.h>

Ensure(strlen_of_hello_is_five) {
    const char *greeting = "Hello";
    int length = strlen(greeting);
    assert_equal_with_message(length, 5, "[%s] should be 5, but was %d", greeting, length);
}

 TestSuite *our_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, strlen_of_hello_is_five);
    return suite;
}

int main(int argc, char **argv) {
    return run_test_suite(our_tests(), create_text_reporter());
}
