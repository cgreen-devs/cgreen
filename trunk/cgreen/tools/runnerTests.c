#include <cgreen/cgreen.h>

#include "runner.c"

Describe(Runner);

BeforeEach(Runner){}

AfterEach(Runner){}

#define CONTEXT_NAME "context"
#define TEST_NAME "test"

static char FUNCTION_NAME[] = CGREEN_SPEC_PREFIX CGREEN_SEPARATOR CONTEXT_NAME CGREEN_SEPARATOR TEST_NAME;
static char FUNCTION_NAME_WITH_DEFAULT_CONTEXT[] = CGREEN_SPEC_PREFIX CGREEN_SEPARATOR CGREEN_DEFAULT_SUITE CGREEN_SEPARATOR TEST_NAME;

Ensure(Runner, can_get_suite_name_from_function) {
    assert_that(suite_name(FUNCTION_NAME), is_equal_to_string(CONTEXT_NAME));
}

Ensure(Runner, can_get_test_name_from_function) {
    assert_that(test_name(FUNCTION_NAME), is_equal_to_string(TEST_NAME));
}

Ensure(Runner, can_mangle_default_context_and_test_name_into_function_name) {
    assert_that(mangle_test_name(TEST_NAME), is_equal_to_string(FUNCTION_NAME_WITH_DEFAULT_CONTEXT));
}

Ensure(Runner, can_mangle_explicit_context_and_test_name_into_function_name) {
    assert_that(mangle_test_name(CONTEXT_NAME ":" TEST_NAME), is_equal_to_string(FUNCTION_NAME));
}

/* vim: set ts=4 sw=4 et cindent: */
/* Local variables: */
/* tab-width: 4     */
/* End:             */
