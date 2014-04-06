#ifndef ASSERTIONS_HEADER
#define ASSERTIONS_HEADER

#include "internal/assertions_internal.h"

#include <cgreen/constraint.h>
#ifdef __cplusplus
#include <cgreen/cpp_assertions.h>
#endif
#include <cgreen/reporter.h>
#include <stdint.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
#include <cgreen/cpp_assertions.h>

namespace cgreen {
    extern "C" {
#endif

/*
  Modern style asserts using constraints:

   assert_that(actual, <constraint>(expected));
   assert_that(<expression>);

*/
#define assert_that(...) assert_that_NARG(__VA_ARGS__)(__VA_ARGS__)
#define assert_that_double(actual, constraint) assert_that_double_(__FILE__, __LINE__, #actual, (double)actual, constraint)

#define pass_test() assert_true(true)
#define fail_test(...) assert_true_with_message(false, __VA_ARGS__)


/* Utility: */
void significant_figures_for_assert_double_are(int figures);


/* Legacy style asserts:*/
#define assert_true(result) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, "[" #result "] should be true\n", NULL)
#define assert_false(result) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, "[" #result "] should be false\n", NULL)
#define assert_equal(tried, expected) \
        assert_equal_(__FILE__, __LINE__, #tried, (intptr_t)tried, (intptr_t)expected)
#define assert_not_equal(tried, expected) \
        assert_not_equal_(__FILE__, __LINE__, #tried, (intptr_t)tried, (intptr_t)expected)
#define assert_double_equal(tried, expected) \
        assert_double_equal_(__FILE__, __LINE__, #tried, tried, expected)
#define assert_double_not_equal(tried, expected) \
        assert_double_not_equal_(__FILE__, __LINE__, #tried, tried, expected)
#define assert_string_equal(tried, expected) \
        assert_string_equal_(__FILE__, __LINE__, #tried, tried, expected)
#define assert_string_not_equal(tried, expected) \
        assert_string_not_equal_(__FILE__, __LINE__, #tried, tried, expected)

#define assert_true_with_message(result, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, result, __VA_ARGS__)
#define assert_false_with_message(result, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! result, __VA_ARGS__)
#define assert_equal_with_message(tried, expected, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried == expected), __VA_ARGS__)
#define assert_not_equal_with_message(tried, expected, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, (tried != expected), __VA_ARGS__)
#define assert_double_equal_with_message(tried, expected, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, doubles_are_equal(tried, expected), __VA_ARGS__)
#define assert_double_not_equal_with_message(tried, expected, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, doubles_are_equal(tried, expected), __VA_ARGS__)
#define assert_string_equal_with_message(tried, expected, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strings_are_equal(tried, expected), __VA_ARGS__)
#define assert_string_not_equal_with_message(tried, expected, ...) \
        (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, !strings_are_equal(tried, expected), __VA_ARGS__)

#ifdef __cplusplus
    }
}
#endif

#endif
