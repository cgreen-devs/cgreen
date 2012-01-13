#include <cgreen/assertions.h>
#include <cgreen/boxed_double.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/reporter.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) > (b) ? (b) : (a))

static void format_message_for(char *message, size_t message_size, Constraint *constraint, const char *actual_string, intptr_t actual);

static double accuracy(int significant_figures, double largest);

static int significant_figures = 8;

void assert_that_(const char *file, int line, const char *actual_string, intptr_t actual, Constraint* constraint) {
    if (NULL != constraint && constraint->type != PARAMETER) {
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                file,
                line,
                false,
                "Got constraint of type [%s], but only parameter constraints are allowed for assertions.",
                constraint->name);

        constraint->destroy(constraint);

        return;
    }

    char assertion_failure_message[255];
    format_message_for(assertion_failure_message, sizeof(assertion_failure_message) - 1,
            constraint, actual_string, actual);

    (*get_test_reporter()->assert_true)(
        get_test_reporter(),
        file,
        line,
        (*constraint->compare)(constraint, actual),
        assertion_failure_message
    );
  
    constraint->destroy(constraint);
}

void assert_that_double_(const char *file, int line, const char *actual_string, double actual, Constraint* constraint) {
    if (NULL != constraint && constraint->type != PARAMETER) {
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                file,
                line,
                false,
                "Got constraint of type [%s], but only parameter constraints are allowed for assertions.",
                constraint->name);

        constraint->destroy(constraint);

        return;
    }

    BoxedDouble* boxed_actual = (BoxedDouble*)box_double(actual);

    (*get_test_reporter()->assert_true)(get_test_reporter(), file, line, (*constraint->compare)(constraint, (intptr_t)boxed_actual),
            "Expected [%s] with actual value [%f] to [%s] [%f] within [%d] significant figures",
            actual_string,
            actual,
            constraint->name,
            as_double(constraint->stored_value),
            significant_figures);

    free(boxed_actual);
    constraint->destroy(constraint);
}

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
            "[%f] should match [%f] within %d significant figures", tried, expected, significant_figures);
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

int strings_are_equal(const char* actual, const char* expected) {
    /* TODO: if expected is null, warn user to use appropriate non-string assert instead */
    if ((actual == NULL) || (expected == NULL)) {
        return (actual == expected);
    }

    return (strcmp(actual, expected) == 0);
}

int string_contains(const char* actual, const char* expected) {
    /* TODO: if expected is null, warn user */
    if ((actual == NULL) || (expected == NULL)) {
        return false;
    }

    return (strstr(actual, expected) != NULL);
}

int doubles_are_equal(double tried, double expected) {
    return max(tried, expected) - min(tried, expected) < accuracy(significant_figures, max(tried, expected));
}

static double accuracy(int figures, double largest) {
    return pow(10, 1 + (int)log10(largest) - figures);
}

static void format_message_for(char *message, size_t message_size, Constraint *constraint, const char *actual_string, intptr_t actual) {
    char actual_value_string[32];
    snprintf(actual_value_string, sizeof(actual_value_string) - 1, "%" PRIdPTR, actual);

    if (constraint == is_null ||
            constraint == is_non_null ||
            constraint == is_true ||
            constraint == is_false) { 
        /* for these highly descriptive, parameterless constraints, 
           we don't print the stored value */
        snprintf(message, message_size - 1, 
                "Expected [%s] to [%s]",
                actual_string,
                constraint->name);
        return;
    }


    if (strings_are_equal(actual_string, actual_value_string) ||
            strings_are_equal(actual_string, "true") ||
            strings_are_equal(actual_string, "false")) {
        /* when the actual string and the value are the same, don't print both of them */
        /* also, don't print "0" for false and "1" for true */
        snprintf(message, message_size - 1,
                "Expected [%s] to [%s] [%" PRIdPTR "]",
                actual_string,
                constraint->name,
                constraint->stored_value);

        return;
    } 

    snprintf(message, message_size - 1,
                "Expected [%s] with actual value [%" PRIdPTR "] to [%s] [%" PRIdPTR "]",
                actual_string,
                actual,
                constraint->name,
                constraint->stored_value);

    return;
}
#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
