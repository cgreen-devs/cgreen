#include <cgreen/mocks.h>
#include <cgreen/constraint.h>
#include <cgreen/reporter.h>
#include <cgreen/vector.h>
#include <cgreen/parameters.h>
#include <stdlib.h>
#include <string.h>

typedef struct RecordedResult_ {
    const char *function;
    intptr_t result;
    int should_keep;
} RecordedResult;

typedef struct RecordedExpectation_ {
    const char *function;
    const char *test_file;
    int test_line;
    int should_keep;
    CgreenVector *constraints;
} RecordedExpectation;

typedef struct UnwantedCall_ {
    const char *test_file;
    int test_line;
    const char *function;
} UnwantedCall;

static CgreenVector *result_queue = NULL;
static CgreenVector *expectation_queue = NULL;
static CgreenVector *unwanted_calls = NULL;

intptr_t stubbed_result(const char *function);
static RecordedResult *create_recorded_result(const char *function, intptr_t result);
static void ensure_result_queue_exists();
static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, va_list constraints);
static void destroy_expectation(void *expectation);
static void ensure_expectation_queue_exists();
static void ensure_unwanted_calls_list_exists();
RecordedResult *find_result(const char *function);
static void unwanted_check(const char *function);
void trigger_unfulfilled_expectations(CgreenVector *expectation_queue, TestReporter *reporter);
RecordedExpectation *find_expectation(const char *function);
void apply_any_constraints(RecordedExpectation *expectation, const char *parameter, intptr_t actual);

intptr_t mock_(const char *function, const char *parameters, ...) {
    unwanted_check(function);
    RecordedExpectation *expectation = find_expectation(function);
    if (expectation != NULL) {
        CgreenVector *names = create_vector_of_names(parameters);
        int i;
        va_list actual;
        va_start(actual, parameters);
        for (i = 0; i < cgreen_vector_size(names); i++) {
            apply_any_constraints(expectation, (const char *)cgreen_vector_get(names, i), va_arg(actual, intptr_t));
        }
        va_end(actual);
        destroy_cgreen_vector(names);
        if (! expectation->should_keep) {
            destroy_expectation(expectation);
        }
    }
    return stubbed_result(function);
}

void expect_(const char *function, const char *test_file, int test_line, ...) {
    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    va_end(constraints);
    expectation->should_keep = 0;
}

void always_expect_(const char *function, const char *test_file, int test_line, ...) {
    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    va_end(constraints);
    expectation->should_keep = 1;
}

void expect_never_(const char *function, const char *test_file, int test_line) {
    ensure_unwanted_calls_list_exists();
    UnwantedCall *unwanted = (UnwantedCall *)malloc(sizeof(UnwantedCall));
    unwanted->test_file = test_file;
    unwanted->test_line = test_line;
    unwanted->function = function;
    cgreen_vector_add(unwanted_calls, unwanted);
}

void will_return_(const char *function, intptr_t result) {
    RecordedResult *record = create_recorded_result(function, result);
    record->should_keep = 0;
}

void always_return_(const char *function, intptr_t result) {
    RecordedResult *record = create_recorded_result(function, result);
    record->should_keep = 1;
}

void clear_mocks() {
    if (result_queue != NULL) {
        destroy_cgreen_vector(result_queue);
        result_queue = NULL;
    }
    if (expectation_queue != NULL) {
        destroy_cgreen_vector(expectation_queue);
        expectation_queue = NULL;
    }
    if (unwanted_calls != NULL) {
        destroy_cgreen_vector(unwanted_calls);
        unwanted_calls = NULL;
    }
}

void tally_mocks(TestReporter *reporter) {
    trigger_unfulfilled_expectations(expectation_queue, reporter);
    clear_mocks();
}

intptr_t stubbed_result(const char *function) {
    RecordedResult *result = find_result(function);
    if (result == NULL) {
        return 0;
    }
    intptr_t value = result->result;
    if (! result->should_keep) {
        free(result);
    }
    return value;
}

static RecordedResult *create_recorded_result(const char *function, intptr_t result) {
    ensure_result_queue_exists();
    RecordedResult *record = (RecordedResult *)malloc(sizeof(RecordedResult));
    record->function = function;
    record->result = result;
    cgreen_vector_add(result_queue, record);
    return record;
}

static void ensure_result_queue_exists() {
    if (result_queue == NULL) {
        result_queue = create_cgreen_vector(&free);
    }
}

static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, va_list constraints) {
    ensure_expectation_queue_exists();
    RecordedExpectation *expectation = (RecordedExpectation *)malloc(sizeof(RecordedExpectation));
    expectation->function = function;
    expectation->test_file = test_file;
    expectation->test_line = test_line;
    expectation->constraints = create_cgreen_vector(&destroy_constraint);
    Constraint *constraint;
    while ((constraint = va_arg(constraints, Constraint *)) != (Constraint *)0) {
        cgreen_vector_add(expectation->constraints, constraint);
    }
    cgreen_vector_add(expectation_queue, expectation);
    return expectation;
}

static void destroy_expectation(void *abstract) {
    RecordedExpectation *expectation = (RecordedExpectation *)abstract;
    destroy_cgreen_vector(expectation->constraints);
    free(expectation);
}

static void ensure_expectation_queue_exists() {
    if (expectation_queue == NULL) {
        expectation_queue = create_cgreen_vector(&destroy_expectation);
    }
}

static void ensure_unwanted_calls_list_exists() {
    if (unwanted_calls == NULL) {
        unwanted_calls = create_cgreen_vector(&free);
    }
}

RecordedResult *find_result(const char *function) {
    int i;
    for (i = 0; i < cgreen_vector_size(result_queue); i++) {
        RecordedResult *result = (RecordedResult *)cgreen_vector_get(result_queue, i);
        if (strcmp(result->function, function) == 0) {
            if (! result->should_keep) {
                return (RecordedResult *) cgreen_vector_remove(result_queue, i);
            }
            return result;
        }
    }
    return NULL;
}

static void unwanted_check(const char *function) {
    int i;
    for (i = 0; i < cgreen_vector_size(unwanted_calls); i++) {
        UnwantedCall *unwanted = (UnwantedCall *) cgreen_vector_get(unwanted_calls, i);
        if (strcmp(unwanted->function, function) == 0) {
            (*get_test_reporter()->assert_true)(
                    get_test_reporter(),
                    unwanted->test_file,
                    unwanted->test_line,
                    0,
                    "Unexpected call to function [%s]", function);
        }
    }
}

void trigger_unfulfilled_expectations(CgreenVector *expect_queue,
        TestReporter *reporter) {
    int i;
    for (i = 0; i < cgreen_vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = cgreen_vector_get(expect_queue, i);
        if (! expectation->should_keep) {
            (*reporter->assert_true)(
                    reporter,
                    expectation->test_file,
                    expectation->test_line,
                    0,
                    "Call was not made to function [%s]", expectation->function);
        }
    }
}

RecordedExpectation *find_expectation(const char *function) {
    int i;
    for (i = 0; i < cgreen_vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation =
                (RecordedExpectation *)cgreen_vector_get(expectation_queue, i);
        if (strcmp(expectation->function, function) == 0) {
            if (! expectation->should_keep) {
                return (RecordedExpectation *) cgreen_vector_remove(expectation_queue, i);
            }
            return expectation;
        }
    }
    return NULL;
}

void apply_any_constraints(RecordedExpectation *expectation, const char *parameter, intptr_t actual) {
    int i;
    for (i = 0; i < cgreen_vector_size(expectation->constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(expectation->constraints, i);
        if (is_constraint_parameter(constraint, parameter)) {
            test_constraint(
                    constraint,
                    expectation->function,
                    actual,
                    expectation->test_file,
                    expectation->test_line,
                    get_test_reporter());
        }
    }
}

/* vim: set ts=4 sw=4 et cindent: */
