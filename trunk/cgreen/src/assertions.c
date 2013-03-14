#include <cgreen/assertions.h>
#include <cgreen/boxed_double.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/message_formatting.h>
#include <cgreen/reporter.h>
#include <cgreen/string_comparison.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif


#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) > (b) ? (b) : (a))

static double accuracy(int significant_figures, double largest);

static int significant_figures = 8;

void assert_that_(const char *file, int line, const char *actual_string, intptr_t actual, Constraint* constraint) {

    char *failure_message;
    if (NULL != constraint && is_not_comparing(constraint)) {
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                file,
                line,
                false,
                "\tGot constraint of type [%s], but they are not allowed for assertions, only in mock expectations.",
                constraint->name);

        constraint->destroy(constraint);

        return;
    }

    if (parameters_are_not_valid_for(constraint, actual)) {
        char *validation_message = validation_failure_message_for(constraint, actual);

        (*get_test_reporter()->assert_true)(
                                            get_test_reporter(),
                                            file,
                                            line,
                                            false,
                                            validation_message);

        constraint->destroy(constraint);
        free(validation_message);
        return;
    }

    failure_message = constraint->failure_message(constraint, actual_string, actual);

    (*get_test_reporter()->assert_true)(
                                        get_test_reporter(),
                                        file,
                                        line,
                                        (*constraint->compare)(constraint, actual),
                                        failure_message
                                        );

    constraint->destroy(constraint);
    free(failure_message);
}

void assert_that_double_(const char *file, int line, const char *expression, double actual, Constraint* constraint) {
    BoxedDouble* boxed_actual;
    if (NULL != constraint && is_not_comparing(constraint)) {
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                file,
                line,
                false,
                "\tGot constraint of type [%s], but they are not allowed for assertions, only in mock expectations.",
                constraint->name);

        constraint->destroy(constraint);

        return;
    }

    boxed_actual = (BoxedDouble*)box_double(actual);

    (*get_test_reporter()->assert_true)(get_test_reporter(), file, line, (*constraint->compare)(constraint, (intptr_t)boxed_actual),
            "Expected [%s] to [%s] [%s] within [%d] significant figures"
            "\t\tactual value:\t%08f\n"
            "\t\texpected value:\t%08f",
            expression,
            constraint->name,
            constraint->expected_value_name,
            significant_figures,
            actual,
            as_double(constraint->expected_value));

    free(boxed_actual);
    constraint->destroy(constraint);
}

void assert_equal_(const char *file, int line, const char *expression, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried == expected),
            "[%s] should be [%d] but was [%d]\n", expression, expected, tried);
}

void assert_not_equal_(const char *file, int line, const char *expression, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried != expected),
            "[%s] should not be [%d] but was\n", expression, expected, tried);
}

void assert_double_equal_(const char *file, int line, const char *expression, double tried, double expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            doubles_are_equal(tried, expected),
            "[%s] should be [%f] within %d significant figures but was [%f]\n", expression, expected, significant_figures, tried);
}

void assert_double_not_equal_(const char *file, int line, const char *expression, double tried, double expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            ! doubles_are_equal(tried, expected),
            "[%s] should not be [%f] within %d significant figures but was [%f]\n", expression, expected, significant_figures, tried);
}

void assert_string_equal_(const char *file, int line, const char *expression, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            strings_are_equal(tried, expected),
            "[%s] should be [%s] but was [%s]\n", expression, show_null_as_the_string_null(expected), show_null_as_the_string_null(tried));
}

void assert_string_not_equal_(const char *file, int line, const char *expression, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            ! strings_are_equal(tried, expected),
            "[%s] should not be [%s] but was\n", expression, show_null_as_the_string_null(expected));
}

void significant_figures_for_assert_double_are(int figures) {
    significant_figures = figures;
}

const char *show_null_as_the_string_null(const char *string) {
    return (string == NULL ? "NULL" : string);
}

bool doubles_are_equal(double tried, double expected) {
    return max(tried, expected) - min(tried, expected) < accuracy(significant_figures, max(tried, expected));
}

static double accuracy(int figures, double largest) {
    return pow(10, 1 + (int)log10(largest) - figures);
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
