#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

TestSuite *all_constraints_tests();

int main(int argc, char **argv) {
    int suite_result;
    TestSuite *suite = create_test_suite();
    add_suite(suite, all_constraints_tests());
    if (argc > 1) {
        suite_result = run_single_test(suite, argv[1], create_text_reporter());
    }
    suite_result = run_test_suite(suite, create_text_reporter());

    destroy_test_suite(suite);

    return suite_result;
}
