#include "../cgreen.h"

TestSuite *messaging_tests();
TestSuite *assertion_tests();
TestSuite *breadcrumb_tests();

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, messaging_tests());
    add_suite(suite, assertion_tests());
    add_suite(suite, breadcrumb_tests());
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
