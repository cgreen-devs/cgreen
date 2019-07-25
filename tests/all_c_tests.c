#include <cgreen/cgreen.h>

/* all_c_tests.c - which also doubles as
   all_cpp_tests.c using symbolic link and CPP conditionals */

#ifdef __cplusplus
using namespace cgreen;
#endif

TestSuite *assertion_tests(void);
TestSuite *breadcrumb_tests(void);
TestSuite *cdash_reporter_tests(void);
TestSuite *collector_tests(void);
TestSuite *constraint_tests(void);
#ifdef __cplusplus
TestSuite *cpp_assertion_tests(void);
#endif
TestSuite *cute_reporter_tests(void);
TestSuite *cgreen_value_tests(void);
TestSuite *message_formatting_tests(void);
TestSuite *messaging_tests(void);
TestSuite *mock_tests(void);
TestSuite *parameter_tests(void);
TestSuite *reflective_no_teardown_tests(void);
TestSuite *reflective_tests(void);
TestSuite *text_reporter_tests(void);
TestSuite *unit_tests(void);
TestSuite *vector_tests(void);
TestSuite *xml_reporter_tests(void);

int main(int argc, char **argv) {
    int suite_result;
    TestSuite *suite = create_named_test_suite("all_c_tests");
    TestReporter *reporter = create_text_reporter(); 

    add_suite(suite, assertion_tests());
    add_suite(suite, breadcrumb_tests());
    add_suite(suite, cgreen_value_tests());
    add_suite(suite, cdash_reporter_tests());
    add_suite(suite, constraint_tests());
#ifdef __cplusplus
    add_suite(suite, cpp_assertion_tests());
#endif
    add_suite(suite, cute_reporter_tests());
    add_suite(suite, message_formatting_tests());
    add_suite(suite, messaging_tests());
    add_suite(suite, mock_tests());
    add_suite(suite, parameter_tests());
    add_suite(suite, reflective_no_teardown_tests());
    add_suite(suite, reflective_tests());
    add_suite(suite, text_reporter_tests());
    add_suite(suite, unit_tests());
    add_suite(suite, vector_tests());
    add_suite(suite, xml_reporter_tests());

    if (argc > 1) {
        suite_result = run_single_test(suite, argv[1], reporter);
    } else {
        suite_result = run_test_suite(suite, reporter);
    }

    destroy_test_suite(suite);
    destroy_reporter(reporter);

    return suite_result;
}
