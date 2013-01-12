#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(context_api);
BeforeEach(context_api) {}
AfterEach(context_api) {}

// SUITES
Ensure(context_api, compiles_suite_api) {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, context_api, compiles_suite_api);

    run_test_suite(suite, NULL);
    run_single_test(suite, "name", NULL);

    die_in(1);

    destroy_test_suite(suite);
}
