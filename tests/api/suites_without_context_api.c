#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

// SUITES
Ensure(a_test) {}

Ensure(suites_compiles) {
    TestSuite *suite = create_test_suite();

    add_test(suite, a_test);
    add_tests(suite, a_test, a_test, a_test);

    run_test_suite(suite, NULL);
    run_single_test(suite, "name", NULL);

    die_in(1);

    destroy_test_suite(suite);
}
