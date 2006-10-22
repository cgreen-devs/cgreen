#ifndef _ASSERTIONS_HEADER_
#define _ASSERTIONS_HEADER_

#define assert_true(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, NULL)
#define assert_false(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, NULL)
#define assert_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried == expected), __VA_ARGS__)
#define assert_not_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried != expected), __VA_ARGS__)
#define assert_string_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strcmp(tried, expected) == 0, __VA_ARGS__)
#define assert_string_not_equal(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strcmp(tried, expected) != 0, __VA_ARGS__)

#define assert_true_with_message(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, __VA_ARGS__)
#define assert_false_with_message(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, __VA_ARGS__)
#define assert_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried == expected), __VA_ARGS__)
#define assert_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried != expected), __VA_ARGS__)
#define assert_string_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strcmp(tried, expected) == 0, __VA_ARGS__)
#define assert_string_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strcmp(tried, expected) != 0, __VA_ARGS__)

#endif
