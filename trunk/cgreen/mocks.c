#include "mocks.h"
#include "constraint.h"
#include "reporter.h"
#include "vector.h"
#include "assertions.h"
#include <stdlib.h>
#include <string.h>

typedef struct _RecordedResult {
    const char *function;
    intptr_t result;
    int should_keep;
} RecordedResult;

typedef struct _UnwantedCall {
    const char *test_file;
    int test_line;
    const char *function;
} UnwantedCall;

static Vector *result_queue = NULL;
static Vector *unwanted_calls = NULL;

typedef struct _TestLocation {
    const char *file;
    int line;
} TestLocation;

static TestLocation location = {"", 0};

static void ensure_result_queue();
static void ensure_unwanted_calls();
static void record_expectation(const char *check_file, int check_line, const char *function, const char *parameter, intptr_t expected);
RecordedResult *find_result(const char *function);
static void unwanted_check(const char *function, const char *parameter);
static void set_test_location(const char *file, int line);
static void clear_test_location();

void _checked_integer(const char *check_file, int check_line, const char *function, const char *parameter, intptr_t integer) {
    unwanted_check(function, parameter);
}

void _checked_string(const char *check_file, int check_line, const char *function, const char *parameter, const char *string) {
    unwanted_check(function, parameter);
}

intptr_t _stubbed_result(const char *function, const char *file, int line) {
    RecordedResult *result = find_result(function);
    if (result == NULL) {
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                file,
                line,
                0,
                "No return value set for function [%s]", function);
        return 0;
    }
    intptr_t value = result->result;
    if (! result->should_keep) {
        free(result);
    }
    return value;
}

void _expect(const char *function, const char *file, int line, ...) {
}

void _expect_never(const char *function, const char *test_file, int test_line) {
    ensure_unwanted_calls();
    UnwantedCall *unwanted = (UnwantedCall *)malloc(sizeof(UnwantedCall));
    unwanted->test_file = test_file;
    unwanted->test_line = test_line;
    unwanted->function = function;
    vector_add(unwanted_calls, unwanted);
}

void _will_return(const char *function, intptr_t result) {
    ensure_result_queue();
    RecordedResult *record = (RecordedResult *)malloc(sizeof(RecordedResult));
    record->function = function;
    record->result = result;
    record->should_keep = 0;
    vector_add(result_queue, record);
}

void _always_return(const char *function, intptr_t result) {
    ensure_result_queue();
    RecordedResult *record = (RecordedResult *)malloc(sizeof(RecordedResult));
    record->function = function;
    record->result = result;
    record->should_keep = 1;
    vector_add(result_queue, record);
}

void clear_mocks() {
    if (result_queue != NULL) {
        destroy_vector(result_queue);
    }
    if (unwanted_calls != NULL) {
        destroy_vector(unwanted_calls);
    }
    clear_test_location();
}

void tally_mocks(TestReporter *reporter) {
    clear_mocks();
}

static void ensure_result_queue() {
    if (result_queue == NULL) {
        result_queue = create_vector(&free);
    }
}

static void ensure_unwanted_calls() {
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

static void unwanted_check(const char *function, const char *parameter) {
    int i;
    for (i = 0; i < vector_size(unwanted_calls); i++) {
        UnwantedCall *unwanted = vector_get(unwanted_calls, i);
        if (strcmp(unwanted->function, function) == 0) {
            (*get_test_reporter()->assert_true)(
                    get_test_reporter(),
                    unwanted->test_file,
                    unwanted->test_line,
                    0,
                    "Unexpected call %s(..., %s, ...)", function, parameter);
        }
    }
}

static void set_test_location(const char *file, int line) {
    location.file = file;
    location.line = line;
}

static void clear_test_location() {
    location.file = "";
    location.line = 0;
}
