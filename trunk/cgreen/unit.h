#ifndef _UNIT_HEADER_
#define _UNIT_HEADER_

#include "reporter.h"
#include "mocks.h"

#define create_test_suite() create_named_test_suite(__func__)
#define add_test(suite, test) _add_unit_test(suite, #test, &test)
#define add_unit_test(suite, test) _add_unit_test(suite, #test, &test)
#define add_suite(owner, suite) _add_suite(owner, #suite, suite)
#define add_test_suite(owner, suite) _add_suite(owner, #suite, suite)
#define setup(suite, function) _setup(suite, &function)
#define teardown(suite, function) _teardown(suite, &function)
#define assert_true(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, __VA_ARGS__)
#define assert_false(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, __VA_ARGS__)
#define assert_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried == expected), __VA_ARGS__)
#define assert_not_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried != expected), __VA_ARGS__)
#define assert_string_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strcmp(tried, expected) == 0, __VA_ARGS__)
#define assert_string_not_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strcmp(tried, expected) != 0, __VA_ARGS__)
#define recorded_integer(expected) _recorded_integer(__FILE__, __LINE__, expected)

typedef struct _TestSuite TestSuite;

TestSuite *create_named_test_suite(char *name);
void destroy_test_suite(TestSuite *suite);
void _add_unit_test(TestSuite *suite, char *name, void (*test)(TestReporter *));
void _add_suite(TestSuite *owner, char *name, TestSuite *suite);
void _setup(TestSuite *suite, void (*set_up)());
void _teardown(TestSuite *suite, void (*tear_down)());
void die_in(unsigned int seconds);
int run_test_suite(TestSuite *suite, TestReporter *reporter);
int run_single_test(TestSuite *suite, char *test, TestReporter *reporter);

#endif
