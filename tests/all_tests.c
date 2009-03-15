#include <cgreen/cgreen.h>

TestSuite *messaging_tests();
TestSuite *assertion_tests();
TestSuite *vector_tests();
TestSuite *constraint_tests();
TestSuite *parameter_tests();
TestSuite *mock_tests();
TestSuite *breadcrumb_tests();
TestSuite *slurp_tests();
TestSuite *cute_reporter_tests();
TestSuite *unit_tests();
TestSuite *collector_tests();

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, messaging_tests());
    add_suite(suite, assertion_tests());
    add_suite(suite, vector_tests());
    add_suite(suite, constraint_tests());
    add_suite(suite, parameter_tests());
    add_suite(suite, mock_tests());
    add_suite(suite, breadcrumb_tests());
    add_suite(suite, slurp_tests());
    add_suite(suite, cute_reporter_tests());
    add_suite(suite, collector_tests());
    add_suite(suite, unit_tests());
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
