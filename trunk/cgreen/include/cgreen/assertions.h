#ifndef ASSERTIONS_HEADER
#define ASSERTIONS_HEADER

#include <inttypes.h>

typedef struct TestContextAssert_ TestContextAssert;

typedef struct TestAsserts_ TestAsserts;
struct TestAsserts_ {
    void (*set_significant_figures)(TestAsserts *, double epsilon);
    double epsilon;
};

TestAsserts *get_test_assert();
TestAsserts *create_assert();
void destroy_assert(TestAsserts *asserts);


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
#define assert_string_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, strings_are_equal(tried, expected), __VA_ARGS__)
#define assert_string_not_equal_with_message(tried, expected, ...) (*get_test_reporter()->assert_true)(get_test_reporter(), __FILE__, __LINE__, ! strings_are_equal(tried, expected), __VA_ARGS__)

#define set_significant_figures(epsilon) (*get_test_assert()->set_significant_figures)(get_test_assert(),epsilon)

void assert_equal_(const char *file, int line, intptr_t tried, intptr_t expected);
void assert_not_equal_(const char *file, int line, intptr_t tried, intptr_t expected);
void assert_double_equal_(const char *file, int line, double tried, double expected);
void assert_double_not_equal_(const char *file, int line, double tried, double expected);
void assert_string_equal_(const char *file, int line, const char *tried, const char *expected);
void assert_string_not_equal_(const char *file, int line, const char *tried, const char *expected);
const char *show_null_as_the_string_null(const char *string);
int strings_are_equal(const char *tried, const char *expected);
int double_are_equal(double tried, double expected, double epsilon);

#endif
