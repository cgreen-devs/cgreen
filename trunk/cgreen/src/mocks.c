#include <assert.h>
#include <cgreen/mocks.h>
#include <cgreen/parameters.h>
// TODO: report PC-Lint bug about undeserved 451
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

typedef struct RecordedExpectation_ {
       const char *function;
       const char *test_file;
       int test_line;
       int time_to_live;
       CgreenVector *constraints;
} RecordedExpectation;

const int UNLIMITED_TIME_TO_LIVE = 0x0f314159;

static CgreenVector *global_expectation_queue = NULL;

static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, va_list constraints);
static void destroy_expectation(RecordedExpectation *expectation);
static void ensure_expectation_queue_exists(void);
void remove_expectation_for(const char *function);
void trigger_unfulfilled_expectations(CgreenVector *expectation_queue, TestReporter *reporter);
RecordedExpectation *find_expectation(const char *function);
void apply_any_parameter_constraints(RecordedExpectation *expectation, const char *parameter, intptr_t actual, TestReporter* test_reporter);
intptr_t stored_result_or_default_for(CgreenVector* constraints);

bool is_always_call(RecordedExpectation* expectation);
bool have_always_expectation_for(const char* function);

bool is_never_call(RecordedExpectation* expectation);
bool have_never_call_expectation_for(const char* function);

void report_violated_never_call(TestReporter*, RecordedExpectation*);
void destroy_expectation_if_time_to_die(RecordedExpectation *expectation);


intptr_t mock_(TestReporter* test_reporter, const char *function, const char *parameters, ...) {

    RecordedExpectation *expectation = find_expectation(function);
    if (expectation == NULL) {
        /* TODO: fail in strict mode */
        return 0;
    }

    if (is_never_call(expectation)) {
        report_violated_never_call(test_reporter, expectation);
        return 0;
    }

    intptr_t stored_result = stored_result_or_default_for(expectation->constraints);

    /* no need to parse parameters if none were provided */
    if (strlen(parameters) == 0) {
        destroy_expectation_if_time_to_die(expectation);
        return stored_result;
    }

    CgreenVector *parameter_names = create_vector_of_names(parameters);
    va_list actuals;
    va_start(actuals, parameters);
    for (int i = 0; i < cgreen_vector_size(parameter_names); i++) {
        const char* parameter_name = (const char*)cgreen_vector_get(parameter_names, i);
        uintptr_t actual = va_arg(actuals, uintptr_t);
        apply_any_parameter_constraints(expectation, parameter_name, actual, test_reporter);
    }
    va_end(actuals);
    destroy_cgreen_vector(parameter_names);

    destroy_expectation_if_time_to_die(expectation);

    return stored_result;
}

Constraint *when_(const char *parameter, Constraint* constraint) {
    constraint->parameter_name = parameter;
    return constraint;
}

void destroy_expectation_if_time_to_die(RecordedExpectation *expectation) {

    if (is_always_call(expectation)) {
        return;
    }

    /* TODO: expectation->decrement_time_to_live_if_necessary(); if (expectation->time_to_die()) { ... } */
    expectation->time_to_live--;

    if (expectation->time_to_live <= 0) {
        remove_expectation_for(expectation->function);
        destroy_expectation(expectation);
    }
}

void expect_(TestReporter* test_reporter, const char *function, const char *test_file, int test_line, ...) {
    if (have_always_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Function [%s] already has an expectation it will always be called a certain way; "
                "any expectations declared after an always expectation are discarded", function);

        return;
    }

    if (have_never_call_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Function [%s] already has an expectation that it will never be called; "
                "any expectations declared after a never call expectation are discarded", function);

        return;
    }


    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    va_end(constraints);
    expectation->time_to_live = 1;
    cgreen_vector_add(global_expectation_queue, expectation);
}

void always_expect_(TestReporter* test_reporter, const char *function, const char *test_file, int test_line, ...) {
    if (have_always_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Function [%s] already has an expectation it will always be called a certain way; "
                "any expectations declared after an always expectation are discarded", function);

        return;
    }

    if (have_never_call_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Function [%s] already has an expectation that it will never be called; "
                "any expectations declared after a never call expectation are discarded", function);

        return;
    }

    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    va_end(constraints);
    expectation->time_to_live = UNLIMITED_TIME_TO_LIVE;
    cgreen_vector_add(global_expectation_queue, expectation);
}

void expect_never_(TestReporter* test_reporter, const char *function, const char *test_file, int test_line, ...) {
    if (have_always_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Function [%s] already has an expectation it will always be called a certain way; "
                "declaring an expectation after an always expectation is not allowed", function);

        return;
    }

    if (have_never_call_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Function [%s] already has an expectation that it will never be called; "
                "declaring an expectation for a function after a never call expectation is not allowed", function);

        return;
    }

    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    expectation->time_to_live = -UNLIMITED_TIME_TO_LIVE;
    cgreen_vector_add(global_expectation_queue, expectation);
}

void report_violated_never_call(TestReporter* test_reporter, RecordedExpectation* expectation) {
    test_reporter->assert_true(
            test_reporter,
            expectation->test_file,
            expectation->test_line,
            false,
            "Function [%s] has an expectation that it will never be called, but it was",
            expectation->function);
}

void clear_mocks() {
    if (global_expectation_queue != NULL) {
        destroy_cgreen_vector(global_expectation_queue);
        global_expectation_queue = NULL;
    }
}

void tally_mocks(TestReporter *reporter) {
    trigger_unfulfilled_expectations(global_expectation_queue, reporter);
    clear_mocks();
}

static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, va_list constraints) {
    ensure_expectation_queue_exists();
    RecordedExpectation *expectation = (RecordedExpectation *)malloc(sizeof(RecordedExpectation));
    expectation->function = function;
    expectation->test_file = test_file;
    expectation->test_line = test_line;
    expectation->constraints = create_cgreen_vector((GenericDestructor)&destroy_constraint);

    Constraint* constraint;
    while ((constraint = va_arg(constraints, Constraint *)) != (Constraint *)0) {
        cgreen_vector_add(expectation->constraints, constraint);
    }

    return expectation;
}

static void destroy_expectation(RecordedExpectation *expectation) {
    destroy_cgreen_vector(expectation->constraints);
    expectation->constraints = NULL;
    expectation->function = NULL;
    expectation->test_file = NULL;
    expectation->test_line = 0;
    expectation->time_to_live = 0;

    free(expectation);
}

static void ensure_expectation_queue_exists() {
    if (global_expectation_queue == NULL) {
        global_expectation_queue = create_cgreen_vector((GenericDestructor)&destroy_expectation);
    }
}

void remove_expectation_for(const char *function) {
    for (int i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
        RecordedExpectation *expectation = (RecordedExpectation *)cgreen_vector_get(global_expectation_queue, i);

        if (NULL == expectation) {
            printf("*** NULL expectation found -- maybe a previous incorrect removal?\n");
            continue;
        }

        if (strcmp(expectation->function, function) == 0) {
            cgreen_vector_remove(global_expectation_queue, i);
            return;
        }
    }
}

void trigger_unfulfilled_expectations(CgreenVector *expectation_queue,
        TestReporter *reporter) {
    for (int i = 0; i < cgreen_vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = (RecordedExpectation *)cgreen_vector_get(expectation_queue, i);

        if (NULL == expectation) {
            printf("*** NULL expectation found -- maybe a previous incorrect removal?\n");
            continue;
        }

	if (is_always_call(expectation) || is_never_call(expectation)) {
            continue;
        }

        if (expectation->time_to_live != 0) {
            (*reporter->assert_true)(
                    reporter,
                    expectation->test_file,
                    expectation->test_line,
                    0,
                    "Expected call was not made to function [%s]", expectation->function);
        }
    }
}

RecordedExpectation *find_expectation(const char *function) {
    for (int i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
        RecordedExpectation *expectation =
                (RecordedExpectation *)cgreen_vector_get(global_expectation_queue, i);
        if (strcmp(expectation->function, function) == 0) {
            return expectation;
        }
    }
    return NULL;
}

void apply_any_parameter_constraints(RecordedExpectation *expectation, const char *parameter, intptr_t actual, TestReporter* test_reporter) {
    for (int i = 0; i < cgreen_vector_size(expectation->constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(expectation->constraints, i);

        if (!constraint_is_for_parameter(constraint, parameter)) {
            continue;
        }

        // TODO: push this into the constraint->test, once the "test" is renamed to something like "execute", and delete here.
        //       this potentially also means that the SET_PARAMETER type could be removed
        if (constraint->type == SET_PARAMETER) {
            memmove((void *)actual, (void *)constraint->expected_value, constraint->size_of_stored_value);
        }

        constraint->test(
            constraint,
            expectation->function,
            actual,
            expectation->test_file,
            expectation->test_line,
            test_reporter);
    }
}

intptr_t stored_result_or_default_for(CgreenVector* constraints) {
    for (int i = 0; i < cgreen_vector_size(constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(constraints, i);
        if (constraint->type == RETURN_VALUE) {
            return constraint->expected_value;
        }
    }

    return 0;
}

bool is_always_call(RecordedExpectation* expectation) {
    return expectation->time_to_live == UNLIMITED_TIME_TO_LIVE;
}

bool have_always_expectation_for(const char* function) {
    for (int i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
        RecordedExpectation *expectation =
                (RecordedExpectation *)cgreen_vector_get(global_expectation_queue, i);
        if (strcmp(expectation->function, function) == 0) {
            if (is_always_call(expectation)) {
                return true;
            }
        }
    }

    return false;
}

bool is_never_call(RecordedExpectation* expectation) {
    return expectation->time_to_live == -UNLIMITED_TIME_TO_LIVE;
}

bool have_never_call_expectation_for(const char* function) {
    for (int i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
        RecordedExpectation *expectation =
                (RecordedExpectation *)cgreen_vector_get(global_expectation_queue, i);
        if (strcmp(expectation->function, function) == 0) {
            if (is_never_call(expectation)) {
                return true;
            }
        }
    }

    return false;
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
