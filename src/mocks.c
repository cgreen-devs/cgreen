#include "mocks.h"
#include "constraint.h"
#include "reporter.h"
#include "vector.h"
#include "parameters.h"
#include <stdlib.h>
#include <string.h>

typedef struct _RecordedResult {
    const char *function;
    intptr_t result;
    int should_keep;
} RecordedResult;

typedef struct _RecordedExpectation {
    const char *function;
    const char *test_file;
    int test_line;
    int should_keep;
    Vector *constraints;
} RecordedExpectation;

typedef struct _UnwantedCall {
    const char *test_file;
    int test_line;
    const char *function;
} UnwantedCall;

static Vector *result_queue = NULL;
static Vector *expectation_queue = NULL;
static Vector *unwanted_calls = NULL;

intptr_t stubbed_result(const char *function);
static RecordedResult *create_recorded_result(const char *function, intptr_t result);
static void ensure_result_queue_exists();
static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, va_list constraints);
static void destroy_expectation(void *expectation);
static void ensure_expectation_queue_exists();
static void ensure_unwanted_calls_list_exists();
RecordedResult *find_result(const char *function);
static void unwanted_check(const char *function);
void trigger_unfulfilled_expectations(Vector *expectation_queue, TestReporter *reporter);
RecordedExpectation *find_expectation(const char *function);
void apply_any_constraints(RecordedExpectation *expectation, const char *parameter, intptr_t actual);

intptr_t _mock(const char *function, const char *parameters, ...) {
    unwanted_check(function);
    RecordedExpectation *expectation = find_expectation(function);
    if (expectation != NULL) {
        Vector *names = create_vector_of_names(parameters);
        int i;
        va_list actual;
        va_start(actual, parameters);
        for (i = 0; i < vector_size(names); i++) {
            apply_any_constraints(expectation, vector_get(names, i), va_arg(actual, intptr_t));
        }
        va_end(actual);
        destroy_vector(names);
    }
    return stubbed_result(function);
}

void _expect(const char *function, const char *test_file, int test_line, ...) {
    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    va_end(constraints);
    expectation->should_keep = 0;
}

void _always_expect(const char *function, const char *test_file, int test_line, ...) {
    va_list constraints;
    va_start(constraints, test_line);
    RecordedExpectation *expectation = create_recorded_expectation(function, test_file, test_line, constraints);
    va_end(constraints);
    expectation->should_keep = 1;
}

void _expect_never(const char *function, const char *test_file, int test_line) {
    ensure_unwanted_calls_list_exists();
    UnwantedCall *unwanted = (UnwantedCall *)malloc(sizeof(UnwantedCall));
    unwanted->test_file = test_file;
    unwanted->test_line = test_line;
    unwanted->function = function;
    vector_add(unwanted_calls, unwanted);
}

void _will_return(const char *function, intptr_t result) {
    RecordedResult *record = create_recorded_result(function, result);
    record->should_keep = 0;
}

void _always_return(const char *function, intptr_t result) {
    RecordedResult *record = create_recorded_result(function, result);
    record->should_keep = 1;
}

void clear_mocks() {
    if (result_queue != NULL) {
        destroy_vector(result_queue);
    }
    if (expectation_queue != NULL) {
        destroy_vector(expectation_queue);
    }
    if (unwanted_calls != NULL) {
        destroy_vector(unwanted_calls);
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
    vector_add(result_queue, record);
    return record;
}

static void ensure_result_queue_exists() {
    if (result_queue == NULL) {
        result_queue = create_vector(&free);
    }
}

static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, va_list constraints) {
    ensure_expectation_queue_exists();
    RecordedExpectation *expectation = (RecordedExpectation *)malloc(sizeof(RecordedExpectation));
    expectation->function = function;
    expectation->test_file = test_file;
    expectation->test_line = test_line;
    expectation->constraints = create_vector(&destroy_constraint);
    Constraint *constraint;
    while ((constraint = va_arg(constraints, Constraint *)) != (Constraint *)0) {
        vector_add(expectation->constraints, constraint);
    }
    vector_add(expectation_queue, expectation);
    return expectation;
}

static void destroy_expectation(void *abstract) {
    RecordedExpectation *expectation = (RecordedExpectation *)abstract;
    destroy_vector(expectation->constraints);
    free(expectation);
}

static void ensure_expectation_queue_exists() {
    if (expectation_queue == NULL) {
        expectation_queue = create_vector(&destroy_expectation);
    }
}

static void ensure_unwanted_calls_list_exists() {
    if (unwanted_calls == NULL) {
        unwanted_calls = create_vector(&free);
    }
}

RecordedResult *find_result(const char *function) {
    int i;
    for (i = 0; i < vector_size(result_queue); i++) {
        RecordedResult *result = (RecordedResult *)vector_get(result_queue, i);
        if (strcmp(result->function, function) == 0) {
            if (! result->should_keep) {
                return vector_remove(result_queue, i);
            }
            return result;
        }
    }
    return NULL;
}

static void unwanted_check(const char *function) {
    int i;
    for (i = 0; i < vector_size(unwanted_calls); i++) {
        UnwantedCall *unwanted = vector_get(unwanted_calls, i);
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

void trigger_unfulfilled_expectations(Vector *expectation_queue, TestReporter *reporter) {
    int i;
    for (i = 0; i < vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = vector_get(expectation_queue, i);
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
    for (i = 0; i < vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = (RecordedExpectation *)vector_get(expectation_queue, i);
        if (strcmp(expectation->function, function) == 0) {
            if (! expectation->should_keep) {
                return vector_remove(expectation_queue, i);
            }
            return expectation;
        }
    }
    return NULL;
}

void apply_any_constraints(RecordedExpectation *expectation, const char *parameter, intptr_t actual) {
    int i;
    for (i = 0; i < vector_size(expectation->constraints); i++) {
        Constraint *constraint = (Constraint *)vector_get(expectation->constraints, i);
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
