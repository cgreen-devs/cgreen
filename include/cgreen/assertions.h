#ifndef _ASSERTIONS_HEADER_
#define _ASSERTIONS_HEADER_

#include <inttypes.h>

#define assert_true(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, NULL)
#define assert_false(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, NULL)
#define assert_equal(tried, expected) _assert_equal(__FILE__, __LINE__, (intptr_t)tried, (intptr_t)expected)
#define assert_not_equal(tried, expected) _assert_not_equal(__FILE__, __LINE__, (intptr_t)tried, (intptr_t)expected)
#define assert_string_equal(tried, expected) _assert_string_equal(__FILE__, __LINE__, tried, expected)
#define assert_string_not_equal(tried, expected) _assert_string_not_equal(__FILE__, __LINE__, tried, expected)

#define assert_true_with_message(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, __VA_ARGS__)
#define assert_false_with_message(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, __VA_ARGS__)
#define assert_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried == expected), __VA_ARGS__)
#define assert_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried != expected), __VA_ARGS__)
#define assert_string_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strings_are_equal(tried, expected), __VA_ARGS__)
#define assert_string_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! strings_are_equal(tried, expected), __VA_ARGS__)

void _assert_equal(const char *file, int line, intptr_t tried, intptr_t expected);
void _assert_not_equal(const char *file, int line, intptr_t tried, intptr_t expected);
void _assert_string_equal(const char *file, int line, const char *tried, const char *expected);
void _assert_string_not_equal(const char *file, int line, const char *tried, const char *expected);
const char *show_null_as_the_string_null(const char *string);
int strings_are_equal(const char *tried, const char *expected);

#endif
