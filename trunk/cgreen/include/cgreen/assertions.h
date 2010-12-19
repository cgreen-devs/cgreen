#ifndef ASSERTIONS_HEADER
#define ASSERTIONS_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/constraint.h>
#include <inttypes.h>
#include <stdbool.h>

#define assert_true(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, NULL)
#define assert_false(result) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, NULL)
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
#define assert_string_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! strings_are_equal(tried, expected), __VA_ARGS__)

#define assert_that(actual, constraint) assert_that_(__FILE__, __LINE__, (intptr_t)actual, constraint);
#define assert_that_double(actual, constraint) assert_that_double_(__FILE__, __LINE__, (double)actual, constraint);


#define pass() assert_true(true)
#define fail(...) assert_true_with_message(false, __VA_ARGS__)

void assert_equal_(const char *file, int line, intptr_t tried, intptr_t expected);
void assert_not_equal_(const char *file, int line, intptr_t tried, intptr_t expected);
void assert_double_equal_(const char *file, int line, double tried, double expected);
void assert_double_not_equal_(const char *file, int line, double tried, double expected);
void assert_string_equal_(const char *file, int line, const char *tried, const char *expected);
void assert_string_not_equal_(const char *file, int line, const char *tried, const char *expected);
void assert_that_(const char *file, int line, intptr_t actual, Constraint *constraint);
void assert_that_double_(const char *file, int line, double actual, Constraint *constraint);

void significant_figures_for_assert_double_are(int figures);
const char *show_null_as_the_string_null(const char *string);
int strings_are_equal(const char *tried, const char *expected);
int string_contains(const char *actual, const char *expected);
int doubles_are_equal(double tried, double expected);

#ifdef __cplusplus
    }
#endif

#endif
