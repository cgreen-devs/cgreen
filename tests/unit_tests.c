#include <cgreen/cgreen.h>
#include <cgreen/unit.h>

#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static TestSuite* suite;

static void unit_tests_setup(void) {
    suite = create_test_suite();
}

static void unit_tests_teardown(void) {
    destroy_test_suite(suite);
}

Describe(Unittests);
BeforeEach(Unittests) {
    unit_tests_setup();
}
AfterEach(Unittests) {
    unit_tests_teardown();
}

Ensure(Unittests, count_tests_return_zero_for_empty_suite) {
	assert_that(count_tests(suite), is_equal_to(0));
}

Ensure(Unittests, count_tests_return_one_for_suite_with_one_testcase) {
	add_test_with_context(suite, Unittests, count_tests_return_one_for_suite_with_one_testcase);
	assert_that(count_tests(suite), is_equal_to(1));
}

Ensure(Unittests, count_tests_return_four_for_four_nested_suite_with_one_testcase_each) {
	TestSuite *suite2 = create_test_suite();
	TestSuite *suite3 = create_test_suite();
	TestSuite *suite4 = create_test_suite();
	add_test_with_context(suite, Unittests, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite, suite2);
	add_test_with_context(suite2, Unittests, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite2, suite3);
	add_test_with_context(suite3, Unittests, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite3, suite4);
	add_test_with_context(suite4, Unittests, count_tests_return_one_for_suite_with_one_testcase);
	assert_that(count_tests(suite), is_equal_to(4));
}

TestSuite *unit_tests(void) {
	TestSuite *suite = create_test_suite();
	set_setup(suite, unit_tests_setup);

	add_test_with_context(suite, Unittests, count_tests_return_zero_for_empty_suite);
	add_test_with_context(suite, Unittests, count_tests_return_one_for_suite_with_one_testcase);
	add_test_with_context(suite, Unittests, count_tests_return_four_for_four_nested_suite_with_one_testcase_each);

	set_teardown(suite, unit_tests_teardown);
	return suite;
}
