#include <cgreen/cgreen.h>
#include <cgreen/unit.h>

#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static TestSuite* suite;

static void unit_tests_setup() {
    suite = create_test_suite();
}

static void unit_tests_teardown() {
    destroy_test_suite(suite);
}

Ensure(count_tests_return_zero_for_empty_suite) {
	assert_equal(count_tests(suite), 0);
}

Ensure(count_tests_return_one_for_suite_with_one_testcase) {
	add_test(suite, count_tests_return_one_for_suite_with_one_testcase);
	assert_equal(count_tests(suite), 1);
}

Ensure(count_tests_return_four_for_four_nested_suite_with_one_testcase_each) {
	TestSuite *suite2 = create_test_suite();
	TestSuite *suite3 = create_test_suite();
	TestSuite *suite4 = create_test_suite();
	add_test(suite, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite, suite2);
	add_test(suite2, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite2, suite3);
	add_test(suite3, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite3, suite4);
	add_test(suite4, count_tests_return_one_for_suite_with_one_testcase);
	assert_equal(count_tests(suite), 4);
}

Ensure(time_out_in_only_one_second) {
    /* this will sometimes fail when using valgrind or running on a CPU-distressed machine */
    die_in(1);
    sleep(10);
}

TestSuite *unit_tests() {
	TestSuite *suite = create_test_suite();
	setup(suite, unit_tests_setup);

	add_test(suite, count_tests_return_zero_for_empty_suite);
	add_test(suite, count_tests_return_one_for_suite_with_one_testcase);
	add_test(suite, count_tests_return_four_for_four_nested_suite_with_one_testcase_each);
    add_test(suite, time_out_in_only_one_second);

    teardown(suite, unit_tests_teardown);
	return suite;
}
