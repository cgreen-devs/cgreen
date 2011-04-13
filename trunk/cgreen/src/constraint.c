#include <cgreen/assertions.h>
#include <cgreen/boxed_double.h>
#include <cgreen/constraint.h>
#include <stdlib.h>
#include <string.h>

Constraint *create_constraint();
void destroy_empty_constraint(Constraint *constraint);
void destroy_static_constraint(Constraint *constraint);
void destroy_double_constraint(Constraint *constraint);

int compare_want_value(Constraint *constraint, intptr_t actual);
void test_want_value(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

int compare_do_not_want_value(Constraint *constraint, intptr_t actual);
void test_do_not_want_value(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_want_contents(Constraint *constraint, intptr_t actual);
static void test_want_contents(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_do_not_want_contents(Constraint *constraint, intptr_t actual);
static void test_do_not_want_contents(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_true(Constraint *constraint, intptr_t actual);
static void test_true(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_want_string(Constraint *constraint, intptr_t actual);
static void test_want_string(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
static int compare_do_not_want_string(Constraint *constraint, intptr_t actual);
static void test_do_not_want_string(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_want_substring(Constraint *constraint, intptr_t actual);
static void test_want_substring(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
static int compare_do_not_want_substring(Constraint *constraint, intptr_t actual);
static void test_do_not_want_substring(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_want_double(Constraint *constraint, intptr_t actual);
static void test_want_double(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
static int compare_do_not_want_double(Constraint *constraint, intptr_t actual);
static void test_do_not_want_double(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

Constraint *create_constraint() {
    Constraint *constraint = (Constraint *)malloc(sizeof(Constraint));

    constraint->destroy = &destroy_empty_constraint;
    return constraint;
}

Constraint *create_parameter_constraint_for(const char *parameter_name) {
    Constraint *constraint = (Constraint *)malloc(sizeof(Constraint));

    constraint->type = PARAMETER;
    constraint->parameter_name = parameter_name;
    constraint->destroy = &destroy_empty_constraint;
    return constraint;
}

void destroy_empty_constraint(Constraint *constraint) {
    constraint->name = NULL;
    constraint->parameter_name = NULL;
    constraint->compare = NULL;
    constraint->test = NULL;
    constraint->destroy = NULL;

    free(constraint);
}

void destroy_static_constraint(Constraint *constraint) {
    /* static constraints helpers (e.g. is_null) act as singletons, and are never destroyed */
    (void)constraint;
}

void destroy_constraint(Constraint *constraint) {
    (*constraint->destroy)(constraint);
}

bool constraint_is_for_parameter(const Constraint *constraint, const char *parameter) {
    if (constraint->type != PARAMETER && constraint->type != SET_PARAMETER) {
        return false;
    }

    return strcmp(constraint->parameter_name, parameter) == 0;
}

void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*constraint->test)(constraint, function, actual, test_file, test_line, reporter);
}

Constraint *when_(const char *parameter, Constraint* constraint) {
    constraint->parameter_name = parameter;
    return constraint;
}

Constraint *create_equal_to_value_constraint(intptr_t value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_value;
    constraint->test = &test_want_value;
    constraint->name = "equal value";
    constraint->stored_value = value_to_match;
    constraint->size_of_stored_value = sizeof(intptr_t);
    return constraint;
}

Constraint *create_not_equal_to_value_constraint(intptr_t value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_value;
    constraint->test = &test_do_not_want_value;
    constraint->name = "not equal";
    constraint->stored_value = value_to_match;
    constraint->size_of_stored_value = sizeof(intptr_t);
    return constraint;
}

Constraint *create_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_contents;
    constraint->test = &test_want_contents;
    constraint->name = "equal contents of";
    constraint->stored_value = (intptr_t)pointer_to_compare;
    constraint->size_of_stored_value = size_to_compare;
    return constraint;
}

Constraint *create_not_equal_to_contents_constraint(void *pointer_to_compare, size_t size_to_compare) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_contents;
    constraint->test = &test_do_not_want_contents;
    constraint->name = "not equal contents of";
    constraint->stored_value = (intptr_t)pointer_to_compare;
    constraint->size_of_stored_value = size_to_compare;
    return constraint;
}

Constraint *create_equal_to_string_constraint(const char* string_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_string;
    constraint->test = &test_want_string;
    constraint->name = "equal string";
    constraint->stored_value = (intptr_t)string_to_match;
    return constraint;
}

Constraint *create_not_equal_to_string_constraint(const char* string_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_string;
    constraint->test = &test_do_not_want_string;
    constraint->name = "not equal string";
    constraint->stored_value = (intptr_t)string_to_match;
    return constraint;
}

Constraint *create_contains_string_constraint(const char* value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_substring;
    constraint->test = &test_want_substring;
    constraint->name = "contain string";
    constraint->stored_value = (intptr_t)value_to_match;
    return constraint;
}

Constraint *create_does_not_contain_string_constraint(const char* value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_substring;
    constraint->test = &test_do_not_want_substring;
    constraint->name = "not contain string";
    constraint->stored_value = (intptr_t)value_to_match;
    return constraint;
}

Constraint *create_equal_to_double_constraint(double value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_double;
    constraint->test = &test_want_double;
    constraint->name = "equal double";
    constraint->stored_value = box_double(value_to_match);
    constraint->destroy = &destroy_double_constraint;

    return constraint;
}

Constraint *create_not_equal_to_double_constraint(double value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_double;
    constraint->test = &test_do_not_want_double;
    constraint->name = "not equal double";
    constraint->stored_value = box_double(value_to_match);
    constraint->destroy = &destroy_double_constraint;

    return constraint;
}

Constraint *create_return_value_constraint(intptr_t value_to_return) {
    Constraint* constraint = create_constraint();
    constraint->type = RETURN_VALUE;

    constraint->compare = &compare_true;
    constraint->test = &test_true;
    constraint->name = "return value";
    constraint->stored_value = value_to_return;

    return constraint;
}

Constraint *create_set_parameter_value_constraint(const char *parameter_name, intptr_t value_to_set, size_t size_to_set) {
    Constraint* constraint = create_constraint();
    constraint->type = SET_PARAMETER;

    constraint->compare = &compare_true;
    constraint->test = &test_true;
    constraint->name = "set parameter value";
    constraint->stored_value = value_to_set;
    constraint->size_of_stored_value = size_to_set;
    constraint->parameter_name = parameter_name;

    return constraint;
}

int compare_want_value(Constraint *constraint, intptr_t actual) {
    return constraint->stored_value == actual;
}

int compare_do_not_want_value(Constraint *constraint, intptr_t actual) {
    return !compare_want_value(constraint, actual);
}

int compare_want_contents(Constraint *constraint, intptr_t actual) {
    /* we can't inspect the contents of a NULL pointer, so comparison always fails */
    if ((void *)actual == NULL) {
        return 0;
    }

    return 0 == memcmp((void *)constraint->stored_value, (void *)actual, constraint->size_of_stored_value);
}

int compare_do_not_want_contents(Constraint *constraint, intptr_t actual) {
    /* we can't inspect the contents of a NULL pointer, so comparison always fails */
    if ((void *)actual == NULL) {
        return 0;
    }

    return !compare_want_contents(constraint, actual);
}

void test_want_value(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%d], but got [%d] in function [%s] parameter [%s]",
            constraint->stored_value,
            actual,
            function,
            constraint->parameter_name);
}

void test_do_not_want_value(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Did not want [%d], but got [%d] in function [%s] parameter [%s]",
            constraint->stored_value,
            actual,
            function,
            constraint->parameter_name);
}

static int find_index_of_difference(char *expected, char *actual, size_t size_to_compare) {
    char *expectedp = expected;
    char *actualp = actual;

    while (size_to_compare--) {
        if (*expectedp++ != *actualp++) {
            return actualp - actual;
        }
    }

    return -1;
}

void test_want_contents(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    if ((void *)actual == NULL) {
        (*reporter->assert_true)(
                reporter,
                test_file,
                test_line,
                false,
                "Wanted contents of pointer parameter [%s] in function [%s] to be the same, but NULL was supplied",
                constraint->parameter_name,
                function);

        return;
    }

    int difference_index = find_index_of_difference((char *)constraint->stored_value, (char *)actual, constraint->size_of_stored_value);

    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted contents of parameter [%s] in function [%s] to be the same, but they start to differ at offset [%d]",
            constraint->parameter_name,
            function,
            difference_index);
}

void test_do_not_want_contents(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    if ((void *)actual == NULL) {
        (*reporter->assert_true)(
                reporter,
                test_file,
                test_line,
                false,
                "Wanted contents of pointer parameter [%s] in function [%s] to be the different, but NULL was supplied",
                constraint->parameter_name,
                function);

        return;
    }

    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted contents of parameter [%s] in function [%s] to be different, but they are the same",
            constraint->parameter_name,
            function);
}

static int compare_want_string(Constraint *constraint, intptr_t actual) {
    return strings_are_equal((const char *)constraint->stored_value, (const char *)actual);
}

static void test_want_string(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%s], but got [%s] in function [%s] parameter [%s]",
            show_null_as_the_string_null((const char *)constraint->stored_value),
            show_null_as_the_string_null((const char *)actual),
            function,
            constraint->parameter_name);
}

static int compare_do_not_want_string(Constraint *constraint, intptr_t actual) {
    return !compare_want_string(constraint, actual);
}

static void test_do_not_want_string(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%s], but got [%s] in function [%s] parameter [%s]",
            show_null_as_the_string_null((const char *)constraint->stored_value),
            show_null_as_the_string_null((const char *)actual),
            function,
            constraint->parameter_name);
}

static int compare_do_not_want_substring(Constraint *constraint, intptr_t actual) {
    return !compare_want_substring(constraint, actual);
}

static void test_do_not_want_substring(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%s] to be contained in [%s] in function [%s] parameter [%s]",
            show_null_as_the_string_null((const char *)constraint->stored_value),
            show_null_as_the_string_null((const char *)actual),
            function,
            constraint->parameter_name);
}

static int compare_want_substring(Constraint *constraint, intptr_t actual) {
    return string_contains((const char *)actual, (const char *)constraint->stored_value);
}

static void test_want_substring(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%s] to be contained in [%s] in function [%s] parameter [%s]",
            show_null_as_the_string_null((const char *)constraint->stored_value),
            show_null_as_the_string_null((const char *)actual),
            function,
            constraint->parameter_name);
}

static int compare_want_double(Constraint *constraint, intptr_t actual) {
    return doubles_are_equal(as_double(constraint->stored_value), as_double(actual));
}

static void test_want_double(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%d], but got [%d] in function [%s] parameter [%s]",
            as_double(constraint->stored_value),
            as_double(actual),
            function,
            constraint->parameter_name);

    (void)unbox_double(actual);
}

static int compare_do_not_want_double(Constraint *constraint, intptr_t actual) {
    return !compare_want_double(constraint, actual);
}

static void test_do_not_want_double(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Did not want [%d], but got [%d] in function [%s] parameter [%s]",
            as_double(constraint->stored_value),
            as_double(actual),
            function,
            constraint->parameter_name);

    (void)unbox_double(actual);
}

void destroy_double_constraint(Constraint *constraint) {
    (void)unbox_double(constraint->stored_value);
    destroy_empty_constraint(constraint);
}

static int compare_true(Constraint *constraint, intptr_t actual) {
    (void)constraint;
    (void)actual;

    return true;
}

static void test_true(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (void)constraint;
    (void)function;
    (void)actual;
    (void)test_file;
    (void)test_line;
    (void)reporter;
}

/* vim: set ts=4 sw=4 et cindent: */
