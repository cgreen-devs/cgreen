#include <cgreen/cgreen.h>
#include <cgreen/unit.h>

#include <stdio.h>
#include <strings.h>

Ensure count_tests_return_zero_for_empty_suite() {
	TestSuite *suite = create_test_suite();
	assert_equal(count_tests(suite), 0);
}

Ensure count_tests_return_one_for_suite_with_one_testcase() {
	TestSuite *suite = create_test_suite();
	add_test(suite, count_tests_return_one_for_suite_with_one_testcase);
	assert_equal(count_tests(suite), 1);
}

Ensure count_tests_return_four_for_four_nested_suite_with_one_testcase_each() {
	TestSuite *suite1 = create_test_suite();
	TestSuite *suite2 = create_test_suite();
	TestSuite *suite3 = create_test_suite();
	TestSuite *suite4 = create_test_suite();
	add_test(suite1, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite1, suite2);
	add_test(suite2, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite2, suite3);
	add_test(suite3, count_tests_return_one_for_suite_with_one_testcase);
	add_suite(suite3, suite4);
	add_test(suite4, count_tests_return_one_for_suite_with_one_testcase);
	assert_equal(count_tests(suite1), 4);
}

TestSuite *unit_tests() {
	TestSuite *suite = create_test_suite();
	add_test(suite, count_tests_return_zero_for_empty_suite);
	add_test(suite, count_tests_return_one_for_suite_with_one_testcase);
	add_test(suite, count_tests_return_four_for_four_nested_suite_with_one_testcase_each);
	return suite;
}
