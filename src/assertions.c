#include <cgreen/assertions.h>
#include <cgreen/reporter.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) > (b) ? (b) : (a))

static double accuracy(int significant_figures, double largest);

static int significant_figures = 8;

void assert_equal_(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried == expected),
            "[%d] should match [%d]", tried, expected);
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
            doubles_are_equal(tried, expected),
            "[%f] should match [%f] within %d significant figures %f", tried, expected, significant_figures);
}

void assert_double_not_equal_(const char *file, int line, double tried, double expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            ! doubles_are_equal(tried, expected),
            "[%f] should not match [%f] within %d significant figures", tried, expected, significant_figures);
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

void significant_figures_for_assert_double_are(int figures) {
    significant_figures = figures;
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

int doubles_are_equal(const double tried, const double expected) {
    return max(tried, expected) - min(tried, expected) < accuracy(significant_figures, max(tried, expected));
}

static double accuracy(int significant_figures, double largest) {
    return pow(10, 1 + (int)log10(largest) - significant_figures);
}
