#include <cgreen/cgreen.h>

#ifdef __cplusplus
namespace cgreen {
#endif

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

Ensure(Runner, can_identify_cgreen_spec) {
    assert_that(is_cgreen_spec("_CgreenSpec__Runner__can_get_test_name_from_function"));
}

Ensure(Runner, can_identify_a_cxx_mangled_cgreen_spec) {
    assert_that(is_cgreen_spec("__ZN6cgreen51CgreenSpec__Runner__can_get_test_name_from_functionE"));
}


#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
/* Local variables: */
/* tab-width: 4     */
/* End:             */
