#include <cgreen/cgreen.h>

#include "runner.c"

Describe(Runner);

BeforeEach(Runner){}

AfterEach(Runner){}

#define SUITE_NAME "the_suite";

static char FUNCTION_NAME[] = CGREEN_SPEC_PREFIX "suite" CGREEN_NAME_SEPARATOR "testname";

Ensure(Runner, can_get_suite_name_from_function) {
	assert_that(suite_name(FUNCTION_NAME), is_equal_to_string("suite"));
}

Ensure(Runner, can_get_test_name_from_function) {
	assert_that(test_name(FUNCTION_NAME), is_equal_to_string("testname"));
}
