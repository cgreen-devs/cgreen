#include <cgreen/assertions.h>
#include <cgreen/boxed_double.h>
#include <cgreen/constraint.h>
#include <cgreen/message_formatting.h>
#include <cgreen/string_comparison.h>
#include <cgreen/vector.h>
#include <inttypes.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__


#ifdef _MSC_VER
#include "wincompat.h"
#endif

#include "constraint_internal.h"
#include "parameters.h"
#include "utils.h"
#include "cgreen_value_internal.h"


#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) > (b) ? (b) : (a))


static int significant_figures = 8;
static double absolute_tolerance = DBL_MIN / 1.0e-8;


static double accuracy(int significant_figures, double largest);

static bool compare_want_greater_value(Constraint *constraint, CgreenValue actual);

static bool compare_want_lesser_value(Constraint *constraint, CgreenValue actual);

static bool compare_want_contents(Constraint *constraint, CgreenValue actual);

static bool compare_do_not_want_contents(Constraint *constraint, CgreenValue actual);

static bool compare_true(Constraint *constraint, CgreenValue actual);
static void test_true(Constraint *constraint, const char *function, CgreenValue actual,
                      const char *test_file, int test_line, TestReporter *reporter);

static bool compare_want_string(Constraint *constraint, CgreenValue actual);
static bool compare_do_not_want_string(Constraint *constraint, CgreenValue actual);
static bool compare_want_substring(Constraint *constraint, CgreenValue actual);
static bool compare_do_not_want_substring(Constraint *constraint, CgreenValue actual);
static bool compare_want_beginning_of_string(Constraint *constraint, CgreenValue actual);
static bool compare_do_not_want_beginning_of_string(Constraint *constraint, CgreenValue actual);
static bool compare_want_end_of_string(Constraint *constraint, CgreenValue actual);
static bool compare_do_not_want_end_of_string(Constraint *constraint, CgreenValue actual);

static bool compare_want_double(Constraint *constraint, CgreenValue actual);
static void test_want_double(Constraint *constraint, const char *function, CgreenValue actual,
                             const char *test_file, int test_line, TestReporter *reporter);
static bool compare_do_not_want_double(Constraint *constraint, CgreenValue actual);
static void test_do_not_want_double(Constraint *constraint, const char *function, CgreenValue actual,
                                    const char *test_file, int test_line, TestReporter *reporter);
static bool compare_want_lesser_double(Constraint *constraint, CgreenValue actual);
static bool compare_want_greater_double(Constraint *constraint, CgreenValue actual);

static void set_contents(Constraint *constraint, const char *function, CgreenValue actual,
                         const char *test_file, int test_line, TestReporter *reporter);

static const char *default_actual_value_message = "\n\t\tactual value:\t\t\t[%" PRIdPTR "]";
static const char *default_expected_value_message = "\t\texpected value:\t\t\t[%" PRIdPTR "]";

static void execute_sideeffect(Constraint *constraint, const char *function, CgreenValue actual,
                         const char *test_file, int test_line, TestReporter *reporter);

Constraint *create_constraint() {
    Constraint *constraint = (Constraint *)malloc(sizeof(Constraint));
    /* TODO: setting this to NULL as an implicit type check :( */
    constraint->parameter_name = NULL;
    constraint->destroy = &destroy_empty_constraint;
    constraint->failure_message = &failure_message_for;
    constraint->expected_value_name = NULL;
    constraint->actual_value_message = default_actual_value_message;
    constraint->expected_value_message = default_expected_value_message;

    return constraint;
}

static Constraint *create_constraint_expecting(CgreenValue expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint();

    constraint->expected_value = expected_value;
    constraint->expected_value_name = string_dup(expected_value_name);

    return constraint;
}

void destroy_empty_constraint(Constraint *constraint) {
    constraint->name = NULL;
    constraint->parameter_name = NULL;
    constraint->compare = NULL;
    constraint->execute = NULL;
    constraint->destroy = NULL;

    if (constraint->expected_value_name != NULL)
        free((void *)constraint->expected_value_name);

    free(constraint);
}

void destroy_static_constraint(Constraint *constraint) {
    /* static constraints helpers (e.g. is_null) act as singletons, and are never destroyed */
    (void)constraint;
}

void destroy_constraint(Constraint *constraint) {
    if (constraint->destroy != NULL)
        (*constraint->destroy)(constraint);
}

void destroy_constraints(va_list constraints) {
    Constraint *constraint = NULL;
    while ((constraint = va_arg(constraints, Constraint *)) != (Constraint *)0) {
        destroy_constraint(constraint);
    }
}

bool constraint_is_for_parameter(const Constraint *constraint, const char *parameter) {
    return !constraint_is_not_for_parameter(constraint, parameter);
}

bool constraint_is_not_for_parameter(const Constraint *constraint, const char *parameter) {
    if (is_not_comparing(constraint) && is_not_content_setting(constraint)) {
        return true;
    }

    return strcmp(constraint->parameter_name, parameter) != 0;
}

Constraint *create_equal_to_value_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_integer_value(expected_value), expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_want_value;
    constraint->execute = &test_want;
    constraint->name = "equal";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}

Constraint *create_equal_to_hexvalue_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_integer_value(expected_value), expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_want_value;
    constraint->execute = &test_want;
    constraint->name = "equal";
    constraint->size_of_expected_value = sizeof(intptr_t);
    constraint->actual_value_message = "\n\t\tactual value:\t\t\t[0x%x]";
    constraint->expected_value_message = "\t\texpected value:\t\t\t[0x%x]";

    return constraint;
}

Constraint *create_not_equal_to_value_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_integer_value(expected_value), expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_do_not_want_value;
    constraint->execute = &test_want;
    constraint->name = "not equal";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}

Constraint *create_less_than_value_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_integer_value(expected_value), expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_want_lesser_value;
    constraint->execute = &test_true;
    constraint->name = "be less than";
    constraint->expected_value_message = "\t\texpected to be less than:\t[%" PRIdPTR "]";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}

Constraint *create_greater_than_value_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_integer_value(expected_value), expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_want_greater_value;
    constraint->execute = &test_true;
    constraint->name = "be greater than";
    constraint->expected_value_message = "\t\texpected to be greater than:\t[%" PRIdPTR "]";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}

Constraint *create_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare, const char *compared_pointer_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_pointer_value(pointer_to_compare), compared_pointer_name);
    constraint->type = CONTENT_COMPARER;

    constraint->compare = &compare_want_contents;
    constraint->execute = &test_want;
    constraint->name = "equal contents of";
    constraint->size_of_expected_value = size_to_compare;

    return constraint;
}

Constraint *create_not_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare, const char *compared_pointer_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_pointer_value(pointer_to_compare), compared_pointer_name);
    constraint->type = CONTENT_COMPARER;

    constraint->compare = &compare_do_not_want_contents;
    constraint->execute = &test_want;
    constraint->name = "not equal contents of";
    constraint->size_of_expected_value = size_to_compare;

    return constraint;
}

Constraint *create_equal_to_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_want_string;
    constraint->execute = &test_want;
    constraint->name = "equal string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to equal:\t\t[\"%s\"]";

    return constraint;
}

Constraint *create_not_equal_to_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_do_not_want_string;
    constraint->execute = &test_want;
    constraint->name = "not equal string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to not equal:\t[\"%s\"]";

    return constraint;
}

Constraint *create_contains_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_want_substring;
    constraint->execute = &test_want;
    constraint->name = "contain string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to contain:\t\t[\"%s\"]";

    return constraint;
}

Constraint *create_does_not_contain_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_do_not_want_substring;
    constraint->execute = &test_want;
    constraint->name = "not contain string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to not contain:\t[\"%s\"]";

    return constraint;
}

Constraint *create_begins_with_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_want_beginning_of_string;
    constraint->execute = &test_want;
    constraint->name = "begin with string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to begin with:\t\t[\"%s\"]";

    return constraint;
}

Constraint *create_does_not_begin_with_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_do_not_want_beginning_of_string;
    constraint->execute = &test_want;
    constraint->name = "not begin with string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to not begin with:\t[\"%s\"]";

    return constraint;
}

Constraint *create_ends_with_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_want_end_of_string;
    constraint->execute = &test_want;
    constraint->name = "end with string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to end with:\t\t[\"%s\"]";

    return constraint;
}

Constraint *create_does_not_end_with_string_constraint(const char* expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_string_value(expected_value), expected_value_name);
    constraint->type = STRING_COMPARER;

    constraint->compare = &compare_do_not_want_end_of_string;
    constraint->execute = &test_want;
    constraint->name = "not end with string";
    constraint->destroy = &destroy_string_constraint;
    constraint->expected_value_message = "\t\texpected to not end with:\t[\"%s\"]";

    return constraint;
}

Constraint *create_equal_to_double_constraint(double expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_double_value(expected_value), expected_value_name);
    constraint->type = DOUBLE_COMPARER;

    constraint->compare = &compare_want_double;
    constraint->execute = &test_want_double;
    constraint->name = "equal double";
    constraint->destroy = &destroy_double_constraint;

    return constraint;
}

Constraint *create_not_equal_to_double_constraint(double expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_double_value(expected_value), expected_value_name);
    constraint->type = DOUBLE_COMPARER;

    constraint->compare = &compare_do_not_want_double;
    constraint->execute = &test_do_not_want_double;
    constraint->name = "not equal double";
    constraint->destroy = &destroy_double_constraint;

    return constraint;
}

Constraint *create_less_than_double_constraint(double expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_double_value(expected_value), expected_value_name);
    constraint->type = DOUBLE_COMPARER;

    constraint->compare = &compare_want_lesser_double;
    constraint->execute = &test_true;
    constraint->name = "be less than double";
    constraint->destroy = &destroy_double_constraint;
    constraint->expected_value_message = "\t\texpected to be less than:\t[%08f]";

    return constraint;
}

Constraint *create_greater_than_double_constraint(double expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint_expecting(make_cgreen_double_value(expected_value), expected_value_name);
    constraint->type = DOUBLE_COMPARER;

    constraint->compare = &compare_want_greater_double;
    constraint->execute = &test_true;
    constraint->name = "be greater than double";
    constraint->destroy = &destroy_double_constraint;
    constraint->expected_value_message = "\t\texpected to be greater than:\t[%08f]";

    return constraint;
}

Constraint *create_return_value_constraint(intptr_t value_to_return) {
    Constraint* constraint = create_constraint();
    constraint->type = RETURN_VALUE;

    constraint->compare = &compare_true;
    constraint->execute = &test_true;
    constraint->name = "return value";
    constraint->expected_value = make_cgreen_integer_value(value_to_return);

    return constraint;
}

Constraint *create_return_by_value_constraint(intptr_t value_to_return, size_t size) {
    intptr_t actual_return = (intptr_t) malloc(size);
    memcpy((void*)actual_return, (void*)value_to_return, size);
    Constraint* constraint = create_constraint();
    constraint->type = RETURN_VALUE;

    constraint->compare = &compare_true;
    constraint->execute = &test_true;
    constraint->name = "return value";
    constraint->expected_value = make_cgreen_by_value((void*)actual_return, size);

    return constraint;
}

Constraint *create_return_double_value_constraint(double value_to_return) {
    Constraint* constraint = create_constraint();
    constraint->type = RETURN_VALUE;

    constraint->compare = &compare_true;
    constraint->execute = &test_true;
    constraint->name = "return value";
    constraint->expected_value = make_cgreen_double_value(value_to_return);

    return constraint;
}

Constraint *create_set_parameter_value_constraint(const char *parameter_name, intptr_t value_to_set, size_t size_to_set) {
    Constraint* constraint = create_constraint();
    constraint->type = CONTENT_SETTER;

    constraint->compare = &compare_true;
    constraint->execute = &set_contents;
    constraint->name = "set parameter value";
    constraint->expected_value = make_cgreen_integer_value(value_to_set);
    constraint->size_of_expected_value = size_to_set;
    constraint->parameter_name = parameter_name;

    return constraint;
}

Constraint *create_with_side_effect_constraint(void (*callback)(void *), void *data) {
    Constraint* constraint = create_constraint();
    constraint->type = CALL;

    constraint->name = "cause side effect";
    constraint->side_effect_callback = callback;
    constraint->side_effect_data = data;
    constraint->execute = &execute_sideeffect;

    return constraint;
}

bool compare_want_value(Constraint *constraint, CgreenValue actual) {
    return constraint->expected_value.value.integer_value == actual.value.integer_value;
}

bool compare_do_not_want_value(Constraint *constraint, CgreenValue actual) {
    return !compare_want_value(constraint, actual);
}

bool compare_want_greater_value(Constraint *constraint, CgreenValue actual) {
    return actual.value.integer_value > constraint->expected_value.value.integer_value ;
}

bool compare_want_lesser_value(Constraint *constraint, CgreenValue actual) {
    return actual.value.integer_value < constraint->expected_value.value.integer_value;
}


bool compare_want_contents(Constraint *constraint, CgreenValue actual) {
    /* We can't inspect the contents of a NULL pointer, so comparison always fails */
    /* TODO: This should really be .pointer_value */
    if ((void *)actual.value.integer_value == NULL) {
        return 0;
    }

    return 0 == memcmp(constraint->expected_value.value.pointer_value,
                       (void *)actual.value.integer_value, constraint->size_of_expected_value);
}

bool compare_do_not_want_contents(Constraint *constraint, CgreenValue actual) {
    /* we can't inspect the contents of a NULL pointer, so comparison always fails */
    if ((void *)actual.value.integer_value == NULL) {
        return 0;
    }

    return !compare_want_contents(constraint, actual);
}

static void set_contents(Constraint *constraint, const char *function, CgreenValue actual,
                         const char *test_file, int test_line, TestReporter *reporter) {
    char *message;
    (void)function;

    /* TODO: should propagate the whole actual */
    if (parameters_are_not_valid_for(constraint, actual.value.integer_value)) {
        message = validation_failure_message_for(constraint, actual.value.integer_value);

        (*reporter->assert_true)(
                reporter,
                test_file,
                test_line,
                false,
                message);

        free(message);
        return;
    }

    memmove((void *)actual.value.integer_value, constraint->expected_value.value.pointer_value, constraint->size_of_expected_value);
}

static void execute_sideeffect(Constraint *constraint, const char *function, CgreenValue actual,
                               const char *test_file, int test_line, TestReporter *reporter) {
    (void)function;
    (void)actual;

    if (constraint->side_effect_callback == NULL) {

        (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            false,
            "no side effect function was set");
    }
    (constraint->side_effect_callback)(constraint->side_effect_data);
}


void test_want(Constraint *constraint, const char *function, CgreenValue actual,
               const char *test_file, int test_line, TestReporter *reporter) {
    char *message;
    char parameter_name_actual_string[255];

    if (parameters_are_not_valid_for(constraint, actual.value.integer_value)) {
        message = validation_failure_message_for(constraint, actual.value.integer_value);

        (*reporter->assert_true)(
                reporter,
                test_file,
                test_line,
                false,
                message);

        free(message);

        return;
    }

    snprintf(parameter_name_actual_string, sizeof(parameter_name_actual_string) - 1, "[%s] parameter in [%s]", constraint->parameter_name, function);
    message = constraint->failure_message(constraint, parameter_name_actual_string, actual.value.integer_value);

    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            message);

    free(message);
}

static bool compare_want_string(Constraint *constraint, CgreenValue actual) {
    return strings_are_equal(constraint->expected_value.value.string_value, actual.value.string_value);
}

static bool compare_do_not_want_string(Constraint *constraint, CgreenValue actual) {
    return !compare_want_string(constraint, actual);
}

static bool compare_do_not_want_substring(Constraint *constraint, CgreenValue actual) {
    return !compare_want_substring(constraint, actual);
}

static bool compare_want_substring(Constraint *constraint, CgreenValue actual) {
    return string_contains(actual.value.string_value, constraint->expected_value.value.string_value);
}


const unsigned int NOT_FOUND = UINT_MAX;

static unsigned int strpos(const char *haystack, const char *needle)
{
    const char *offset = strstr(haystack, needle);
    if (offset != NULL) {
      return offset - haystack;
    }

    return NOT_FOUND;
}

static bool compare_want_beginning_of_string(Constraint *constraint, CgreenValue actual) {
    return strpos(actual.value.string_value, constraint->expected_value.value.string_value) == 0;
}

static bool compare_do_not_want_beginning_of_string(Constraint *constraint, CgreenValue actual) {
    return strpos(actual.value.string_value, constraint->expected_value.value.string_value) != 0;
}

static bool compare_want_end_of_string(Constraint *constraint, CgreenValue actual) {
    return strpos(actual.value.string_value, constraint->expected_value.value.string_value) ==
        strlen(actual.value.string_value) - strlen(constraint->expected_value.value.string_value);
}

static bool compare_do_not_want_end_of_string(Constraint *constraint, CgreenValue actual) {
    return strpos(actual.value.string_value, constraint->expected_value.value.string_value) !=
        strlen(actual.value.string_value) - strlen(constraint->expected_value.value.string_value);
}



// Double

static bool compare_want_double(Constraint *constraint, CgreenValue actual) {
    return doubles_are_equal(constraint->expected_value.value.double_value, actual.value.double_value);
}

static bool compare_want_lesser_double(Constraint *constraint, CgreenValue actual) {
    return double_is_lesser(constraint->expected_value.value.double_value, actual.value.double_value);
}

static bool compare_want_greater_double(Constraint *constraint, CgreenValue actual) {
    return double_is_greater(constraint->expected_value.value.double_value, actual.value.double_value);
}

static void test_want_double(Constraint *constraint, const char *function, CgreenValue actual,
                             const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%f], but got [%f] in function [%s] parameter [%s]",
            constraint->expected_value.value.double_value,
            actual.value.double_value,
            function,
            constraint->parameter_name);
}

static bool compare_do_not_want_double(Constraint *constraint, CgreenValue actual) {
    return !compare_want_double(constraint, actual);
}

static void test_do_not_want_double(Constraint *constraint, const char *function, CgreenValue actual,
                                    const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Did not want [%f], but got [%f] in function [%s] parameter [%s]",
            constraint->expected_value.value.double_value,
            actual.value.double_value,
            function,
            constraint->parameter_name);
}

void destroy_double_constraint(Constraint *constraint) {
    destroy_empty_constraint(constraint);
}

void destroy_string_constraint(Constraint *constraint) {
    destroy_cgreen_value(constraint->expected_value);
    destroy_empty_constraint(constraint);
}

static bool compare_true(Constraint *constraint, CgreenValue actual) {
    (void)constraint;
    (void)actual;

    return true;
}

static void test_true(Constraint *constraint, const char *function, CgreenValue actual,
                      const char *test_file, int test_line, TestReporter *reporter) {
    (void)constraint;
    (void)function;
    (void)actual;
    (void)test_file;
    (void)test_line;
    (void)reporter;
}

bool values_are_strings_in(const Constraint *constraint) {
    return is_string_comparing(constraint) &&
        (constraint->expected_value.value.string_value != NULL);
}

bool no_expected_value_in(const Constraint *constraint) {
    return strlen(constraint->expected_value_message) == 0;
}

bool is_content_comparing(const Constraint *constraint) {
    return constraint->type == CONTENT_COMPARER;
}

bool is_content_setting(const Constraint *constraint) {
    return constraint->type == CONTENT_SETTER;
}

bool is_not_content_setting(const Constraint *constraint) {
    return !is_content_setting(constraint);
}

bool is_string_comparing(const Constraint *constraint) {
    return constraint->type == STRING_COMPARER;
}

bool is_double_comparing(const Constraint *constraint) {
    return constraint->type == DOUBLE_COMPARER;
}

bool is_comparing(const Constraint *constraint) {
    return is_string_comparing(constraint) ||
            is_content_comparing(constraint) ||
            is_double_comparing(constraint) ||
            constraint->type == VALUE_COMPARER;
}

bool is_not_comparing(const Constraint *constraint) {
    return !is_comparing(constraint);
}

bool is_parameter(const Constraint *constraint) {
    return is_comparing(constraint) || is_content_setting(constraint);
}

bool constraint_is_for_parameter_in(const Constraint *constraint, const char *names) {
    int i;
    bool found = false;

    CgreenVector *parameter_names = create_vector_of_names(names);
    if (!is_parameter(constraint)) return false;

    for (i = 0; i < cgreen_vector_size(parameter_names); i++) {
        const char *mock_parameter_name = (const char *)cgreen_vector_get(parameter_names, i);

        if (constraint_is_for_parameter(constraint, mock_parameter_name)) {
            found = true;
            break;
        }
    }

    destroy_cgreen_vector(parameter_names);
    return found;
}

bool doubles_are_equal(double tried, double expected) {
    double abs_diff = fabs(tried - expected);
    if (abs_diff < absolute_tolerance) return true;
    return abs_diff < accuracy(significant_figures, max(fabs(tried), fabs(expected)));
}

bool double_is_lesser(double actual, double expected) {
    return expected < actual + accuracy(significant_figures, max(actual, expected));
}

bool double_is_greater(double actual, double expected) {
    return expected > actual - accuracy(significant_figures, max(actual, expected));
}

static double accuracy(int figures, double largest) {
    return pow(10.0, 1.0 + floor(log10(fabs(largest))) - figures);
}

void significant_figures_for_assert_double_are(int figures) {
    significant_figures = figures;
}

int get_significant_figures() {
    return significant_figures;
}

/* vim: set ts=4 sw=4 et cindent: */
