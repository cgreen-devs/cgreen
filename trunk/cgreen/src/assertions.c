#include <cgreen/assertions.h>
#include <cgreen/reporter.h>
#include <stdlib.h>
#include <math.h>

struct TestContextAssert_ {
    TestAsserts *assert;
};

static TestContextAssert contextassert;

TestAsserts *get_test_assert() {
    return contextassert.assert;
}

static void set_significant_figures(TestAsserts *assert, double epsilon);

TestAsserts *create_assert() {
    TestAsserts *assert = (TestAsserts *)malloc(sizeof(TestAsserts));
    assert->set_significant_figures = &set_significant_figures;
    assert->epsilon = 0.1;
    contextassert.assert = assert;
    return assert;
}

void destroy_assert(TestAsserts *assert) {
    free(assert);
    contextassert.assert = NULL;
}

void assert_equal_(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried == expected),
            "[%d] should match [%d]", tried, expected);
}

static void set_significant_figures(TestAsserts *assert, double epsilon) {
	assert->epsilon = epsilon;
}

void assert_not_equal_(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried != expected),
            "[%d] should not match [%d]", tried, expected);
}

void assert_double_equal_(const char *file, int line, double tried, double expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            double_are_equal(tried, expected, get_test_assert()->epsilon),
            "[%f] should match [%f]", tried, expected);
}

void assert_double_not_equal_(const char *file, int line, double tried, double expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            ! double_are_equal(tried, expected, get_test_assert()->epsilon),
            "[%f] should not match [%f]", tried, expected);
}

void assert_string_equal_(const char *file, int line, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            strings_are_equal(tried, expected),
            "[%s] should match [%s]", show_null_as_the_string_null(tried), show_null_as_the_string_null(expected));
}

void assert_string_not_equal_(const char *file, int line, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            ! strings_are_equal(tried, expected),
            "[%s] should not match [%s]", show_null_as_the_string_null(tried), show_null_as_the_string_null(expected));
}

const char *show_null_as_the_string_null(const char *string) {
    return (string == NULL ? "NULL" : string);
}

int strings_are_equal(const char *tried, const char *expected) {
    if ((tried == NULL) || (expected == NULL)) {
        return (tried == expected);
    } else {
        return (strcmp(tried, expected) == 0);
    }
}

int double_are_equal(double tried, double expected, double epsilon) {
    int exponent;
    double delta;
    double difference;

    frexp(fabs(tried) > fabs(expected) ? tried : expected, &exponent);
    delta = ldexp(epsilon, exponent);
    difference = tried - expected;
    //printf("x1: %f x2: %f epsilon: %f, \tDelta: %f \tDifference: %f\n", tried,expected, epsilon, delta, difference);

    if (difference > delta)
        return 0; /* x1 > x2 */
    else if (difference < -delta)
        return 0;  /* x1 < x2 */
    else /* -delta <= difference <= delta */
       return 1;  /* x1 == x2 */
}
