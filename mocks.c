#include "mocks.h"
#include "reporter.h"
#include "vector.h"
#include "assertions.h"
#include <stdlib.h>
#include <string.h>

enum {playing = 1, recording};
static int recording_state = playing;
static int should_keep = 0;

typedef struct _RecordedResult {
    const char *function;
    intptr_t result;
    int should_keep;
} RecordedResult;

typedef struct _RecordedExpectation {
    const char *check_file;
    int check_line;
    const char *test_file;
    int test_line;
    const char *function;
    const char *parameter;
    intptr_t expected;
    int should_keep;
} RecordedExpectation;

typedef struct _UnwantedCall {
    const char *test_file;
    int test_line;
    const char *function;
} UnwantedCall;

static Vector *result_queue = NULL;
static Vector *expectation_queue = NULL;
static Vector *unwanted_calls = NULL;

typedef struct _TestLocation {
    const char *file;
    int line;
} TestLocation;

static TestLocation location = {"", 0};

static void ensure_result_queue();
static void ensure_expectation_queue();
static void ensure_unwanted_calls();
static void record_expectation(const char *check_file, int check_line, const char *function, const char *parameter, intptr_t expected);
RecordedExpectation *find_expectation(const char *check_file, int check_line, const char *parameter);
RecordedResult *find_result(const char *function);
static int matches_expectation(RecordedExpectation *expectation, const char *check_file, int check_line, const char *parameter);
static void unwanted_check(const char *function, const char *parameter);
static void set_test_location(const char *file, int line);
static void clear_test_location();
static int is_recording();

void _checked_integer(const char *check_file, int check_line, const char *function, const char *parameter, intptr_t integer) {
    if (is_recording()) {
        record_expectation(check_file, check_line, function, parameter, integer);
    } else {
        unwanted_check(function, parameter);
        RecordedExpectation *expectation = find_expectation(check_file, check_line, parameter);
        if (expectation == NULL) {
            return;
        }
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                expectation->test_file,
                expectation->test_line,
                expectation->expected == integer,
                "%s(..., %s, ...) is [%d], and should be [%d]", function, parameter, integer, (int)(expectation->expected));
        if (! expectation->should_keep) {
            free(expectation);
        }
    }
}

void _checked_string(const char *check_file, int check_line, const char *function, const char *parameter, const char *string) {
    if (is_recording()) {
        record_expectation(check_file, check_line, function, parameter, (intptr_t)string);
    } else {
        unwanted_check(function, parameter);
        RecordedExpectation *expectation = find_expectation(check_file, check_line, parameter);
        if (expectation == NULL) {
            return;
        }
        (*get_test_reporter()->assert_true)(
                get_test_reporter(),
                expectation->test_file,
                expectation->test_line,
                strings_are_equal((char *)(expectation->expected), string),
                "%s(..., %s, ...) is [%s], and should be [%s]",
                function,
                parameter,
                show_null_as_the_string_null(string),
                show_null_as_the_string_null((char *)(expectation->expected)));
        if (! expectation->should_keep) {
            free(expectation);
        }
    }
}

intptr_t _stubbed_result(const char *function, const char *file, int line) {
    if (is_recording()) {
        return 0;
    }
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

void _expect(const char *function, const char *test_file, int test_line) {
    recording_state = recording;
    set_test_location(test_file, test_line);
}

void _expect_never(const char *function) {
    ensure_unwanted_calls();
    UnwantedCall *unwanted = (UnwantedCall *)malloc(sizeof(UnwantedCall));
    unwanted->test_file = location.file;
    unwanted->test_line = location.line;
    unwanted->function = function;
    vector_add(unwanted_calls, unwanted);
}

void _play() {
    recording_state = playing;
    should_keep = 0;
    clear_test_location();
}

void _will_return(const char *function, intptr_t result) {
    ensure_result_queue();
    RecordedResult *record = (RecordedResult *)malloc(sizeof(RecordedResult));
    record->function = function;
    record->result = result;
    record->should_keep = should_keep;
    vector_add(result_queue, record);
}

void _mask(const char *parameters) {
}

void _always() {
    should_keep = 1;
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
    clear_test_location();
}

void tally_mocks(TestReporter *reporter) {
    int i;
    for (i = 0; i < vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = vector_get(expectation_queue, i);
        if (! expectation->should_keep) {
            (*get_test_reporter()->assert_true)(
                    get_test_reporter(),
                    expectation->test_file,
                    expectation->test_line,
                    0,
                    "Unused %s(..., %s, ...) expectation", expectation->function, expectation->parameter);
        }
    }
    clear_mocks();
}

static void ensure_result_queue() {
    if (result_queue == NULL) {
        result_queue = create_vector(&free);
    }
}

static void ensure_expectation_queue() {
    if (expectation_queue == NULL) {
        expectation_queue = create_vector(&free);
    }
}

static void ensure_unwanted_calls() {
    if (unwanted_calls == NULL) {
        unwanted_calls = create_vector(&free);
    }
}

static void record_expectation(const char *check_file, int check_line, const char *function, const char *parameter, intptr_t expected) {
    ensure_expectation_queue();
    RecordedExpectation *expectation = (RecordedExpectation *)malloc(sizeof(RecordedExpectation));
    expectation->check_file = check_file;
    expectation->check_line = check_line;
    expectation->test_file = location.file;
    expectation->test_line = location.line;
    expectation->function = function;
    expectation->parameter = parameter;
    expectation->expected = expected;
    expectation->should_keep = should_keep;
    vector_add(expectation_queue, expectation);
}

RecordedExpectation *find_expectation(const char *check_file, int check_line, const char *parameter) {
    int i;
    for (i = 0; i < vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = (RecordedExpectation *)vector_get(expectation_queue, i);
        if (matches_expectation(expectation, check_file, check_line, parameter)) {
            if (! expectation->should_keep) {
                return vector_remove(expectation_queue, i);
            }
            return expectation;
        }
    }
    return NULL;
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

static int matches_expectation(RecordedExpectation *expectation, const char *check_file, int check_line, const char *parameter) {
    return (strcmp(expectation->check_file, check_file) == 0) &&
            (expectation->check_line == check_line) &&
            (strcmp(expectation->parameter, parameter) == 0);
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

static int is_recording() {
    return (recording_state == recording);
}
