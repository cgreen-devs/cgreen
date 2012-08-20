#ifndef ASSERTIONS_HEADER
#define ASSERTIONS_HEADER

#include <cgreen/constraint.h>
#include <cgreen/string_comparison.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <cgreen/cpp_assertions.h>
#else
#include <cgreen/c_assertions.h>
#endif

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#define assert_true(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, "[" #result "] should be true", NULL)
#define assert_false(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, "[" #result "] should be false", NULL)
#define assert_equal(tried, expected) assert_equal_(__FILE__, __LINE__, (intptr_t)tried, (intptr_t)expected)
#define assert_not_equal(tried, expected) assert_not_equal_(__FILE__, __LINE__, (intptr_t)tried, (intptr_t)expected)
#define assert_double_equal(tried, expected) assert_double_equal_(__FILE__, __LINE__, tried, expected)
#define assert_double_not_equal(tried, expected) assert_double_not_equal_(__FILE__, __LINE__, tried, expected)
#define assert_string_equal(tried, expected) assert_string_equal_(__FILE__, __LINE__, tried, expected)
#define assert_string_not_equal(tried, expected) assert_string_not_equal_(__FILE__, __LINE__, tried, expected)

#define assert_true_with_message(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, __VA_ARGS__)
#define assert_false_with_message(result, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, __VA_ARGS__)
#define assert_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried == expected), __VA_ARGS__)
#define assert_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried != expected), __VA_ARGS__)
#define assert_double_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, doubles_are_equal(tried, expected), __VA_ARGS__)
#define assert_double_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, doubles_are_equal(tried, expected), __VA_ARGS__)
#define assert_string_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strings_are_equal(tried, expected), __VA_ARGS__)
#define assert_string_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, !strings_are_equal(tried, expected), __VA_ARGS__)

#define assert_that_double(actual, constraint) assert_that_double_(__FILE__, __LINE__, #actual, (double)actual, constraint)

#define pass() assert_true(true)
#define fail(...) assert_true_with_message(false, __VA_ARGS__)

void assert_equal_(const char *file, int line, intptr_t tried, intptr_t expected);
void assert_not_equal_(const char *file, int line, intptr_t tried, intptr_t expected);
void assert_double_equal_(const char *file, int line, double tried, double expected);
void assert_double_not_equal_(const char *file, int line, double tried, double expected);
void assert_string_equal_(const char *file, int line, const char *tried, const char *expected);
void assert_string_not_equal_(const char *file, int line, const char *tried, const char *expected);
void assert_that_double_(const char *file, int line, const char *actual_string, double actual, Constraint *constraint);

void significant_figures_for_assert_double_are(int figures);
const char *show_null_as_the_string_null(const char *string);
int doubles_are_equal(double tried, double expected);

#ifdef __cplusplus
    }
}
#endif

#endif
