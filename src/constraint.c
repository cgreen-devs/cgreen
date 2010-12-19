#include <cgreen/assertions.h>
#include <cgreen/boxed_double.h>
#include <cgreen/constraint.h>
#include <stdlib.h>
#include <string.h>

static int compare_want(Constraint *constraint, intptr_t actual);
static int compare_do_not_want(Constraint *constraint, intptr_t actual);
static void test_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
static void test_do_not_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

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

static Constraint *create_constraint();
static void destroy_empty_constraint(Constraint *constraint);
static void destroy_double_constraint(Constraint *constraint);

static Constraint *create_constraint() {
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

static void destroy_empty_constraint(Constraint *constraint) {
    constraint->name = NULL;
    constraint->parameter_name = NULL;
    constraint->stored_value = 0;
    constraint->compare = NULL;
    constraint->test = NULL;
    constraint->destroy = NULL;
    constraint->stored_value = 0;

    free(constraint);
}

void destroy_constraint(void *abstract) {
    Constraint *constraint = (Constraint *)abstract;
    (*constraint->destroy)(constraint);
}

bool constraint_is_for_parameter(const Constraint *constraint, const char *parameter) {
	if (constraint->type != PARAMETER) {
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

Constraint* create_equal_to_intptr_constraint(intptr_t value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want;
    constraint->test = &test_want;
    constraint->name = "equal";
    constraint->stored_value = value_to_match;
    return constraint;
}

Constraint* create_not_equal_to_intptr_constraint(intptr_t value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want;
    constraint->test = &test_do_not_want;
    constraint->name = "not equal";
    constraint->stored_value = value_to_match;
    return constraint;
}

Constraint* create_equal_to_string_constraint(char* value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_string;
    constraint->test = &test_want_string;
    constraint->name = "equal string";
    constraint->stored_value = (intptr_t)value_to_match;
    return constraint;
}

Constraint* create_not_equal_to_string_constraint(char* value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_string;
    constraint->test = &test_do_not_want_string;
    constraint->name = "not equal string";
    constraint->stored_value = (intptr_t)value_to_match;
    return constraint;
}

Constraint* create_contains_string_constraint(char* value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_substring;
    constraint->test = &test_want_substring;
    constraint->name = "contain string";
    constraint->stored_value = (intptr_t)value_to_match;
    return constraint;
}

Constraint* create_does_not_contain_string_constraint(char* value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_substring;
    constraint->test = &test_do_not_want_substring;
    constraint->name = "not contain string";
    constraint->stored_value = (intptr_t)value_to_match;
    return constraint;
}

Constraint* create_equal_to_double_constraint(double value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_want_double;
    constraint->test = &test_want_double;
    constraint->name = "equal double";
    constraint->stored_value = box_double(value_to_match);
    constraint->destroy = &destroy_double_constraint;

    return constraint;
}

Constraint* create_not_equal_to_double_constraint(double value_to_match) {
    Constraint *constraint = create_constraint();
    constraint->type = PARAMETER;

    constraint->compare = &compare_do_not_want_double;
    constraint->test = &test_do_not_want_double;
    constraint->name = "not equal double";
    constraint->stored_value = box_double(value_to_match);
    constraint->destroy = &destroy_double_constraint;

    return constraint;
}

Constraint* create_return_value_constraint(intptr_t value_to_return) {
    Constraint* constraint = create_constraint();
    constraint->type = RETURN_VALUE;

    constraint->compare = &compare_true;
    constraint->test = &test_true;
    constraint->name = "return value";
    constraint->stored_value = value_to_return;

    return constraint;
}

/* NOTE: while returning BoxedDouble* here seems logical, it makes usage messier */
intptr_t box_double(double value) {
    BoxedDouble *box = (BoxedDouble *) malloc(sizeof(BoxedDouble));
    box->value = value;
    return (intptr_t)box;
}

static int compare_want(Constraint *constraint, intptr_t actual) {
    return (constraint->stored_value == actual);
}

static int compare_do_not_want(Constraint *constraint, intptr_t actual) {
    return (constraint->stored_value != actual);
}

static void test_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
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

static void test_do_not_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
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
    return !strings_are_equal((const char *)constraint->stored_value, (const char *)actual);
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
    return !string_contains((const char *)actual, (const char *)constraint->stored_value);
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
    return !doubles_are_equal(as_double(constraint->stored_value), as_double(actual));
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

static void destroy_double_constraint(Constraint *constraint) {
    (void)unbox_double(constraint->stored_value);
    destroy_empty_constraint(constraint);
}

double unbox_double(intptr_t box) {
    double value = as_double(box);
    free((BoxedDouble *)box);
    return value;
}

double as_double(intptr_t box) {
    return ((BoxedDouble *)box)->value;
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
