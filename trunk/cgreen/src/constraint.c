#include <cgreen/constraint.h>
#include <cgreen/assertions.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

static void destroy_empty_constraint(Constraint *constraint);
static int compare_want(Constraint *constraint, intptr_t actual);
static void test_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_non_null(Constraint *constraint, intptr_t actual);
static void test_non_null(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_want_string(Constraint *constraint, intptr_t actual);
static void test_want_string(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);

static int compare_want_double(Constraint *constraint, intptr_t actual);
static void test_want_double(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);
static Constraint *create_constraint(const char *parameter);
static void destroy_double_constraint(Constraint *constraint);
static double unbox_double(intptr_t box);
static double as_double(intptr_t box);

static int compare_using_matcher(Constraint *constraint, intptr_t actual);
static void test_with_matcher(Constraint *constraint, const char *function, const char* matcher_name, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter);


void destroy_constraint(void *abstract) {
    Constraint *constraint = (Constraint *)abstract;
    (*constraint->destroy)(constraint);
}

int is_constraint_parameter(Constraint *constraint, const char *parameter) {
    return strcmp(constraint->parameter, parameter) == 0;
}

void test_constraint(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*constraint->test)(constraint, function, actual, test_file, test_line, reporter);
}

Constraint *want_(const char *parameter, intptr_t expected) {
    Constraint *constraint = create_constraint(parameter);
    constraint->parameter = parameter;
    constraint->compare = &compare_want;
    constraint->test = &test_want;
    constraint->expected = expected;
    return constraint;
}

Constraint *want_non_null_(const char *parameter) {
    Constraint *constraint = create_constraint(parameter);
    constraint->parameter = parameter;
    constraint->compare = &compare_non_null;
    constraint->test = &test_non_null;
    return constraint;
}


Constraint *want_string_(const char *parameter, char *expected) {
    Constraint *constraint = create_constraint(parameter);
    constraint->parameter = parameter;
    constraint->compare = &compare_want_string;
    constraint->test = &test_want_string;
    constraint->expected = (intptr_t)expected;
    return constraint;
}

static int compare_non_null(Constraint *constraint, intptr_t actual) {
    return (NULL != (void*)actual);
}

static void test_non_null(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted non-null, but got null in function [%s] parameter [%s]",
            function,
            constraint->parameter);
}

Constraint *want_double_(const char *parameter, intptr_t expected) {
    Constraint *constraint = (Constraint *)malloc(sizeof(Constraint));
    constraint->parameter = parameter;
    constraint->destroy = &destroy_double_constraint;
    constraint->compare = &compare_want_double;
    constraint->test = &test_want_double;
    constraint->expected = expected;
    return constraint;
}

intptr_t box_double(double d) {
    BoxedDouble *box = (BoxedDouble *) malloc(sizeof(BoxedDouble));
    box->d = d;
    return (intptr_t)box;
}

Constraint *with_(const char *parameter, const char* matcher_name, int (*matcher_function)(const void*)) {
	Constraint *constraint = create_constraint(parameter);

    constraint->parameter = parameter;
    constraint->compare = &compare_using_matcher;
    constraint->test = &test_with_matcher;
    constraint->name = matcher_name;
    constraint->expected = (intptr_t)matcher_function;
    return constraint;
}

static void destroy_empty_constraint(Constraint *constraint) {
    free(constraint);
}

static int compare_want(Constraint *constraint, intptr_t actual) {
    return (constraint->expected == actual);
}

static void test_want(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%d], but got [%d] in function [%s] parameter [%s]",
            constraint->expected,
            actual,
            function,
            constraint->parameter);
}

static int compare_want_string(Constraint *constraint, intptr_t actual) {
    return strings_are_equal((const char *)constraint->expected, (const char *)actual);
}

static void test_want_string(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%s], but got [%s] in function [%s] parameter [%s]",
            show_null_as_the_string_null((const char *)constraint->expected),
            show_null_as_the_string_null((const char *)actual),
            function,
            constraint->parameter);
}

static int compare_want_double(Constraint *constraint, intptr_t actual) {
    return doubles_are_equal(as_double(constraint->expected), as_double(actual));
}

static void test_want_double(Constraint *constraint, const char *function, intptr_t actual, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Wanted [%d], but got [%d] in function [%s] parameter [%s]",
            as_double(constraint->expected),
            as_double(actual),
            function,
            constraint->parameter);
    unbox_double(actual);
}

static int compare_using_matcher(Constraint *constraint, intptr_t actual) {
	int (*matches)(const void*) = constraint->expected;
    return matches(actual);
}

static void test_with_matcher(Constraint *constraint, const char *function, const char* matcher_name, intptr_t matcher_function, const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, matcher_function),
            "Wanted parameter [%s] to match [%s] in function [%s]",
            constraint->parameter,
            matcher_name,
            function);
}

static Constraint *create_constraint(const char *parameter) {
    Constraint *constraint = (Constraint *)malloc(sizeof(Constraint));
    constraint->parameter = parameter;
    constraint->destroy = &destroy_empty_constraint;
    return constraint;
}

static void destroy_double_constraint(Constraint *constraint) {
    unbox_double(constraint->expected);
    destroy_empty_constraint(constraint);
}

static double unbox_double(intptr_t box) {
    double d = as_double(box);
    free((BoxedDouble *)box);
    return d;
}

static double as_double(intptr_t box) {
    return ((BoxedDouble *)box)->d;
}

/* vim: set ts=4 sw=4 et cindent: */
