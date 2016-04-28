#include <cgreen/cgreen.h>

using namespace cgreen;

// TestSuite *messaging_tests();
// TestSuite *assertion_tests();
TestSuite *cpp_assertion_tests();
// TestSuite *vector_tests();
// TestSuite *constraint_tests();
// TestSuite *parameter_tests();
// TestSuite *mock_tests();
// TestSuite *breadcrumb_tests();
// TestSuite *cute_reporter_tests();
// TestSuite *cdash_reporter_tests();
// TestSuite *unit_tests();
// TestSuite *message_formatting_tests();
// TestSuite *collector_tests();
// TestSuite *reflective_tests();
// TestSuite *reflective_no_teardown_tests();

int main(int argc, char **argv) {
    int suite_result;
    TestSuite *suite = create_named_test_suite("all_cpp_tests");
    TestReporter *reporter = create_text_reporter(); 
    add_suite(suite, cpp_assertion_tests());
    // add_suite(suite, messaging_tests());
    // add_suite(suite, message_formatting_tests());
    // add_suite(suite, assertion_tests());
    // add_suite(suite, vector_tests());
    // add_suite(suite, constraint_tests());
    // add_suite(suite, parameter_tests());
    // add_suite(suite, mock_tests());
    // add_suite(suite, breadcrumb_tests());
    // add_suite(suite, cute_reporter_tests());
    // add_suite(suite, cdash_reporter_tests());
    // add_suite(suite, unit_tests());
    // add_suite(suite, reflective_tests());
    // add_suite(suite, reflective_no_teardown_tests());
    if (argc > 1) {
        suite_result = run_single_test(suite, argv[1], reporter);
    } else {
        suite_result = run_test_suite(suite, reporter);
    }

    destroy_test_suite(suite);
    destroy_reporter(reporter);

    return suite_result;
}
