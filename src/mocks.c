#include <assert.h>
#include <cgreen/breadcrumb.h>
#include <cgreen/mocks.h>
#include <cgreen/boxed_double.h>
#include <inttypes.h>
// TODO: report PC-Lint bug about undeserved 451
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgreen_value_internal.h"
#include "parameters.h"
#include "constraint_internal.h"


#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

typedef struct RecordedExpectation_ {
    const char *function;
    const char *test_file;
    int test_line;
    int time_to_live;
    CgreenVector *constraints;
} RecordedExpectation;

const int UNLIMITED_TIME_TO_LIVE = 0x0f314159;

static CgreenMockMode cgreen_mocks_are_ = strict_mocks;
static CgreenVector *learned_mock_calls = NULL;
static CgreenVector *successfully_mocked_calls = NULL;
static CgreenVector *global_expectation_queue = NULL;

static CgreenVector *create_vector_of_actuals(va_list actuals, int count);
static CgreenVector *create_equal_value_constraints_for(CgreenVector *parameter_names,
                                                        CgreenVector *actual_values);
static CgreenVector *create_constraints_vector(void);
static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file,
                                                        int test_line, CgreenVector *constraints);
static CgreenVector *constraints_vector_from_va_list(va_list constraints);
static void destroy_expectation(RecordedExpectation *expectation);
static void ensure_expectation_queue_exists(void);
static void ensure_learned_mock_calls_list_exists(void);
static void ensure_successfully_mocked_calls_list_exists(void);
static void remove_expectation_for(const char *function);
static void trigger_unfulfilled_expectations(CgreenVector *expectation_queue, TestReporter *reporter);
static RecordedExpectation *find_expectation(const char *function);
static void apply_any_read_only_parameter_constraints(RecordedExpectation *expectation,
                                                      const char *parameter,
                                                      CgreenValue actual,
                                                      TestReporter* test_reporter);
static void apply_any_content_setting_parameter_constraints(RecordedExpectation *expectation,
                                                     const char *parameter,
                                                     CgreenValue actual,
                                                     TestReporter* test_reporter);
static CgreenValue stored_result_or_default_for(CgreenVector* constraints);
int number_of_parameter_constraints_in(const CgreenVector* constraints);
static int number_of_parameters_in(const char *parameter_list);
static bool is_always_call(RecordedExpectation* expectation);
static bool have_always_expectation_for(const char* function);
static bool is_never_call(RecordedExpectation* expectation);
static bool have_never_call_expectation_for(const char* function);

static void report_violated_never_call(TestReporter*, RecordedExpectation*);
void report_unexpected_call(TestReporter*, RecordedExpectation*);
void report_mock_parameter_name_not_found(TestReporter *test_reporter, RecordedExpectation *expectation, const char *parameter);
void destroy_expectation_if_time_to_die(RecordedExpectation *expectation);

void cgreen_mocks_are(CgreenMockMode mock_mode) {
    cgreen_mocks_are_ = mock_mode;
}

static int number_of_parameters_in(const char *parameter_list) {
    int count = 1;
    const char *current = parameter_list;

    if (strlen(parameter_list) == 0) return 0;

    while (*current != '\0') {
        if (*current == ',') count++;
        current++;
    }

    return count;
}

int number_of_parameter_constraints_in(const CgreenVector* constraints) {
    int i, parameters = 0;

    for (i = 0; i < cgreen_vector_size(constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(constraints, i);

        if (is_comparing(constraint)) {
           parameters++;
        }
    }

    return parameters;
}

static void learn_mock_call_for(const char *function, const char *mock_file, int mock_line,
                                CgreenVector *parameter_names, CgreenVector *actual_values) {
    CgreenVector *constraints = create_equal_value_constraints_for(parameter_names, actual_values);

    RecordedExpectation *expectation = create_recorded_expectation(function, mock_file, mock_line,
                                                                   constraints);
    ensure_learned_mock_calls_list_exists();
    cgreen_vector_add(learned_mock_calls, (void*)expectation);
}

void handle_missing_expectation_for(const char *function, const char *mock_file, int mock_line,
                                    CgreenVector *parameter_names, CgreenVector *actual_values,
                                    TestReporter *test_reporter) {
    RecordedExpectation *expectation;
    CgreenVector *no_constraints;

    switch (cgreen_mocks_are_) {
    case loose_mocks:
        break;

    case learning_mocks:
        learn_mock_call_for(function, mock_file, mock_line, parameter_names, actual_values);
        break;

    case strict_mocks:
        no_constraints = create_constraints_vector();
        expectation = create_recorded_expectation(function, mock_file, mock_line, no_constraints);
        report_unexpected_call(test_reporter, expectation);

        destroy_expectation(expectation);
        break;
    }
}


static CgreenValue convert_boxed_double_to_cgreen_value(CgreenValue actual) {
    actual.type = DOUBLE;
    actual.value.double_value = unbox_double(actual.value.integer_value);
    return actual;
}

static bool parameter_is_boxed_double(CgreenVector *double_markers, int i) {
    return *(bool*)cgreen_vector_get(double_markers, i) == true;
}


static void convert_boxed_doubles_to_cgreen_values(const char *parameters, CgreenVector *actual_values) {
    CgreenVector *double_markers;
    int i;
    /* Since the caller must use 'box_double()' to pass doubles as
       arguments to 'mock()' we can know which ones are such parameters
       to be able to convert them to CgreenValues here */
    double_markers = create_vector_of_double_markers_for(parameters);
    for (i = 0; i < cgreen_vector_size(double_markers); i++) {
        if (parameter_is_boxed_double(double_markers, i)) {
            CgreenValue *actual = cgreen_vector_get(actual_values, i);
            *actual = convert_boxed_double_to_cgreen_value(*actual);
        }
    }
    destroy_cgreen_vector(double_markers);
}


intptr_t mock_(TestReporter* test_reporter, const char *function, const char *mock_file, int mock_line, const char *parameters, ...) {
    va_list actuals;
    CgreenVector *actual_values;
    CgreenVector *parameter_names;
    int failures_before_read_only_constraints_executed;
    int failures_after_read_only_constraints_executed;
    int i;
    CgreenValue stored_result;
    RecordedExpectation *expectation = find_expectation(function);

    parameter_names = create_vector_of_names(parameters);

    va_start(actuals, parameters);
    actual_values = create_vector_of_actuals(actuals, number_of_parameters_in(parameters));
    va_end(actuals);

    convert_boxed_doubles_to_cgreen_values(parameters, actual_values);

    if (expectation == NULL) {
        handle_missing_expectation_for(function, mock_file, mock_line, parameter_names, actual_values, test_reporter);
        destroy_cgreen_vector(actual_values);
        destroy_cgreen_vector(parameter_names);
        return 0;
    }

    if (is_never_call(expectation)) {
        report_violated_never_call(test_reporter, expectation);
        destroy_cgreen_vector(actual_values);
        destroy_cgreen_vector(parameter_names);
        return 0;
    }

    ensure_successfully_mocked_calls_list_exists();
    cgreen_vector_add(successfully_mocked_calls, (void*)function);

    stored_result = stored_result_or_default_for(expectation->constraints);
    // FIXME: Should verify that return value is not a DOUBLE as `mock_()' can not
    // return them. There should also be a 'mock_double_()' which does the same except
    // returning a double.

    for (i = 0; i < cgreen_vector_size(expectation->constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(expectation->constraints, i);

        if (!is_parameter(constraint)) continue;

        if (!constraint_is_for_parameter_in(constraint, parameters)) {
            // if expectation parameter name isn't in parameter_names,
            // fail test and skip applying constraints unlikely to match
            report_mock_parameter_name_not_found(test_reporter, expectation, constraint->parameter_name);
            destroy_expectation_if_time_to_die(expectation);
            destroy_cgreen_vector(actual_values);
            destroy_cgreen_vector(parameter_names);

            return stored_result.value.integer_value;
        }
    }

    // if read-only constraints aren't matching, content-setting ones might corrupt memory
    // so apply read-only ones first, and if they don't fail, then do the deeper constraints
    failures_before_read_only_constraints_executed = test_reporter->failures;

    for (i = 0; i < cgreen_vector_size(parameter_names); i++) {
        const char* parameter_name = (const char*)cgreen_vector_get(parameter_names, i);
        CgreenValue actual = *(CgreenValue*)cgreen_vector_get(actual_values, i);
        apply_any_read_only_parameter_constraints(expectation, parameter_name, actual, test_reporter);
    }

    failures_after_read_only_constraints_executed = test_reporter->failures;

    // FIXME: this comparison doesn't work because only parent
    //        processes' pass/fail counts are updated, and even then
    //        only once they read from the pipe
    if (failures_before_read_only_constraints_executed == failures_after_read_only_constraints_executed) {
        for (i = 0; i < cgreen_vector_size(parameter_names); i++) {
            const char* parameter_name = (const char*)cgreen_vector_get(parameter_names, i);
            CgreenValue actual = *(CgreenValue*)cgreen_vector_get(actual_values, i);
            apply_any_content_setting_parameter_constraints(expectation, parameter_name, actual, test_reporter);
        }
    }

    destroy_cgreen_vector(parameter_names);
    destroy_cgreen_vector(actual_values);

    destroy_expectation_if_time_to_die(expectation);

    if (stored_result.type == DOUBLE) {
#ifdef V2
        /* TODO: for v2 we should ensure that the user is not trying to return a double
           through 'mock()' when there is a 'mock_double()' available, which there isn't yet.
           So then

               return unbox_double(mock(...));

           should be replaced by

               return mock_double(...);
        */
        test_reporter->assert_true(test_reporter,
                                   mock_file,
                                   mock_line,
                                   false,
                                   "Mocked function [%s] have a 'will_return_double()' expectation, "
                                   "but 'mock()' cannot return doubles; Use 'mock_double()' instead",
                                   function);
        /* But we'll return it anyway, whatever it becomes is what he will get... */
        return stored_result.value.double_value;
#else
        /* ... but for now return a boxed double since the user is probably using

               return unbox_double(mock(...));

           as is the standard way in 1.x
        */
        return box_double(stored_result.value.double_value);
#endif
    } else
        return stored_result.value.integer_value;
}


static CgreenVector *create_vector_of_actuals(va_list actuals, int count) {
    int i;
    CgreenVector *actual_values = create_cgreen_vector(NULL);
    for (i = 0; i < count; i++) {
        uintptr_t actual = va_arg(actuals, uintptr_t);
        cgreen_vector_add(actual_values, (void*)create_cgreen_value(make_cgreen_integer_value(actual)));
    }
    return actual_values;
}


static CgreenVector *create_equal_value_constraints_for(CgreenVector *parameter_names,
                                                        CgreenVector *actual_values) {
    int i;
    CgreenVector *constraints = create_constraints_vector();
    for (i = 0; i < cgreen_vector_size(parameter_names); i++) {
        const char* parameter_name = (const char*)cgreen_vector_get(parameter_names, i);
        CgreenValue actual = *(CgreenValue*)cgreen_vector_get(actual_values, i);
        Constraint *constraint = create_equal_to_value_constraint(actual.value.integer_value,
                                                                  parameter_name);
        cgreen_vector_add(constraints, constraint);
    }
    return constraints;
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
    va_list constraints;
    RecordedExpectation *expectation;
    CgreenVector *constraints_vector;

    if (have_always_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Mocked function [%s] already has an expectation that it will always be called a certain way; "
                "any expectations declared after an always expectation are invalid", function);
        va_start(constraints, test_line);
        destroy_constraints(constraints);
        va_end(constraints);

        return;
    }

    if (have_never_call_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Mocked function [%s] already has an expectation that it will never be called; "
                "any expectations declared after a never call expectation are invalid", function);
        va_start(constraints, test_line);
        destroy_constraints(constraints);
        va_end(constraints);

        return;
    }


    va_start(constraints, test_line);
    constraints_vector = constraints_vector_from_va_list(constraints);
    expectation = create_recorded_expectation(function, test_file, test_line, constraints_vector);
    va_end(constraints);
    expectation->time_to_live = 1;
    cgreen_vector_add(global_expectation_queue, expectation);
}

void always_expect_(TestReporter* test_reporter, const char *function, const char *test_file, int test_line, ...) {
    va_list constraints;
    RecordedExpectation *expectation;
    CgreenVector *constraints_vector;

    if (have_always_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Mocked function [%s] already has an expectation and will always be called a certain way; "
                "any expectations declared after an always expectation are discarded", function);
        va_start(constraints, test_line);
        destroy_constraints(constraints);
        va_end(constraints);

        return;
    }

    if (have_never_call_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Mocked function [%s] already has an expectation that it will never be called; "
                "any expectations declared after a never call expectation are discarded", function);
        va_start(constraints, test_line);
        destroy_constraints(constraints);
        va_end(constraints);

        return;
    }

    va_start(constraints, test_line);
    constraints_vector = constraints_vector_from_va_list(constraints);
    expectation = create_recorded_expectation(function, test_file, test_line, constraints_vector);
    va_end(constraints);
    expectation->time_to_live = UNLIMITED_TIME_TO_LIVE;
    cgreen_vector_add(global_expectation_queue, expectation);
}

void never_expect_(TestReporter* test_reporter, const char *function, const char *test_file, int test_line, ...) {
    va_list constraints;
    RecordedExpectation *expectation;
    CgreenVector *constraints_vector;

    if (have_always_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Mocked function [%s] already has an expectation and will always be called a certain way; "
                "declaring an expectation after an always expectation is not allowed", function);
        va_start(constraints, test_line);
        destroy_constraints(constraints);
        va_end(constraints);

        return;
    }

    if (have_never_call_expectation_for(function)) {
        test_reporter->assert_true(
                test_reporter,
                test_file,
                test_line,
                false,
                "Mocked function [%s] already has an expectation that it will never be called; "
                "declaring an expectation for a function after a never call expectation is not allowed", function);
        va_start(constraints, test_line);
        destroy_constraints(constraints);
        va_end(constraints);

        return;
    }

    va_start(constraints, test_line);
    constraints_vector = constraints_vector_from_va_list(constraints);
    expectation = create_recorded_expectation(function, test_file, test_line, constraints_vector);
    expectation->time_to_live = -UNLIMITED_TIME_TO_LIVE;
    cgreen_vector_add(global_expectation_queue, expectation);
}


static void report_violated_never_call(TestReporter *test_reporter, RecordedExpectation* expectation) {
    test_reporter->assert_true(
            test_reporter,
            expectation->test_file,
            expectation->test_line,
            false,
            "Mocked function [%s] has an expectation that it will never be called, but it was",
            expectation->function);
}


static bool successfully_mocked_call(const char *function_name) {
    int i;

    for (i = 0; i < cgreen_vector_size(successfully_mocked_calls); i++) {
        const char *successfully_mocked_function_name = (const char *)cgreen_vector_get(successfully_mocked_calls, i);
        if (strcmp(successfully_mocked_function_name, function_name) == 0) {
            return true;
        }
    }

    return false;
}


void report_unexpected_call(TestReporter *test_reporter, RecordedExpectation* expectation) {
    const char *message;
    if (successfully_mocked_call(expectation->function)) {
       message = "Mocked function [%s] was called too many times";
    } else {
       message = "Mocked function [%s] did not have an expectation that it would be called";
    }

    test_reporter->assert_true(
        test_reporter,
        expectation->test_file,
        expectation->test_line,
        false,
        message,
        expectation->function);
}

void clear_mocks(void) {
    if (global_expectation_queue != NULL) {
        destroy_cgreen_vector(global_expectation_queue);
        global_expectation_queue = NULL;
    }

    if (learned_mock_calls != NULL) {
        int i;
        for (i = 0; i < cgreen_vector_size(learned_mock_calls); i++) {
            RecordedExpectation *expectation = (RecordedExpectation*)cgreen_vector_get(learned_mock_calls, i);
            destroy_expectation(expectation);
        }

        destroy_cgreen_vector(learned_mock_calls);
        learned_mock_calls = NULL;
    }
}

static void show_breadcrumb(const char *name, void *memo) {
	if (*(int *) memo > 1) {
		fprintf(stderr, "-> ");
	}
	if (*(int *) memo > 0) {
		fprintf(stderr, "%s ", name);
	}
	(*(int *) memo)++;
}


void print_learned_mocks(void) {
    int e, c, i = 0;
    CgreenBreadcrumb *breadcrumb = get_test_reporter()->breadcrumb;

    walk_breadcrumb(breadcrumb, &show_breadcrumb, (void *) &i);

    fprintf(stderr, ": Learned mocks are\n");

    if (cgreen_vector_size(learned_mock_calls) == 0) {
        fprintf(stderr, "\t<none>\n");
        return;
    }

    for (e = 0; e < cgreen_vector_size(learned_mock_calls); e++) {
        RecordedExpectation *expectation = (RecordedExpectation*)cgreen_vector_get(learned_mock_calls, e);
        const char *function_name = expectation->function;
        fprintf(stderr, "\texpect(%s", function_name);
        for (c = 0; c < cgreen_vector_size(expectation->constraints); c++) {
            Constraint *constraint = (Constraint *)cgreen_vector_get(expectation->constraints, c);
            fprintf(stderr, ", when(%s, is_equal_to(%" PRIdPTR "))", constraint->expected_value_name,
                    constraint->expected_value.value.integer_value);
        }
        fprintf(stderr, ");\n");
    }
}

void tally_mocks(TestReporter *reporter) {
    if (cgreen_mocks_are_ == learning_mocks) {
        print_learned_mocks();
    }

    trigger_unfulfilled_expectations(global_expectation_queue, reporter);
    clear_mocks();
}


static CgreenVector *create_constraints_vector(void) {
    return create_cgreen_vector((GenericDestructor)&destroy_constraint);
}


static CgreenVector *constraints_vector_from_va_list(va_list constraints) {
    CgreenVector *vector = create_constraints_vector();
    Constraint *constraint;
    while ((constraint = va_arg(constraints, Constraint *)) != (Constraint *)0) {
        cgreen_vector_add(vector, constraint);
    }
    return vector;
}


static RecordedExpectation *create_recorded_expectation(const char *function, const char *test_file, int test_line, CgreenVector *constraints) {
    RecordedExpectation *expectation;

    ensure_expectation_queue_exists();
    expectation = (RecordedExpectation *)malloc(sizeof(RecordedExpectation));
    expectation->function = function;
    expectation->test_file = test_file;
    expectation->test_line = test_line;
    expectation->constraints = constraints;

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

static void ensure_successfully_mocked_calls_list_exists(void) {
    if (successfully_mocked_calls == NULL) {
        // successfully_mocked_calls are __func__, so there's nothing to destroy
        successfully_mocked_calls = create_cgreen_vector(NULL);
    }
}

static void ensure_learned_mock_calls_list_exists(void) {
    if (learned_mock_calls == NULL) {
        // learned_mock_calls are __func__, so there's nothing to destroy
        learned_mock_calls = create_cgreen_vector(NULL);
    }
}

static void ensure_expectation_queue_exists(void) {
    if (global_expectation_queue == NULL) {
        global_expectation_queue = create_cgreen_vector((GenericDestructor)&destroy_expectation);
    }
}

static void remove_expectation_for(const char *function) {
    int i;
    for (i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
        RecordedExpectation *expectation = (RecordedExpectation *)cgreen_vector_get(global_expectation_queue, i);

        if (NULL == expectation) {
            fprintf(stderr, "*** CGREEN: NULL expectation found -- maybe a previous incorrect removal?");
            continue;
        }

        if (strcmp(expectation->function, function) == 0) {
            cgreen_vector_remove(global_expectation_queue, i);
            return;
        }
    }
}

static void trigger_unfulfilled_expectations(CgreenVector *expectation_queue, TestReporter *reporter) {
    int i;
    for (i = 0; i < cgreen_vector_size(expectation_queue); i++) {
        RecordedExpectation *expectation = (RecordedExpectation *)cgreen_vector_get(expectation_queue, i);

        if (NULL == expectation) {
            fprintf(stderr, "*** NULL unfulfilled cgreen expectation found -- maybe a previous incorrect removal?");
            continue;
        }

        if (is_always_call(expectation) || is_never_call(expectation)) {
            continue;
        }

        (*reporter->assert_true)(
                reporter,
                expectation->test_file,
                expectation->test_line,
                0,
                "Expected call was not made to mocked function [%s]", expectation->function);
    }
}

static RecordedExpectation *find_expectation(const char *function) {
    int i;
    for (i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
        RecordedExpectation *expectation =
            (RecordedExpectation *)cgreen_vector_get(global_expectation_queue, i);

        if (strcmp(expectation->function, function) == 0) {
            return expectation;
        }
    }
    return NULL;
}

void report_mock_parameter_name_not_found(TestReporter *test_reporter, RecordedExpectation *expectation, const char *constraint_parameter_name) {

    test_reporter->assert_true(
        test_reporter,
        expectation->test_file,
        expectation->test_line,
        false,
        "Mocked function [%s] did not define a parameter named [%s]. Did you misspell it in the expectation or forget it in the mock's argument list?",
        expectation->function,
        constraint_parameter_name);
}

static void apply_any_read_only_parameter_constraints(RecordedExpectation *expectation,
                                                      const char *parameter,
                                                      CgreenValue actual,
                                                      TestReporter* test_reporter) {
    int i;

    for (i = 0; i < cgreen_vector_size(expectation->constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(expectation->constraints, i);

        if (constraint_is_not_for_parameter(constraint, parameter)) {
            continue;
        }

        if (constraint->type == CONTENT_SETTER) {
            continue;
        }

        constraint->execute(
            constraint,
            expectation->function,
            actual,
            expectation->test_file,
            expectation->test_line,
            test_reporter);
    }
}

static void apply_any_content_setting_parameter_constraints(RecordedExpectation *expectation, const char *parameter, CgreenValue actual, TestReporter* test_reporter) {
    int i;
    for (i = 0; i < cgreen_vector_size(expectation->constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(expectation->constraints, i);

        if (constraint_is_not_for_parameter(constraint, parameter)) {
            continue;
        }

        if (constraint->type != CONTENT_SETTER) {
            continue;
        }

        constraint->execute(
            constraint,
            expectation->function,
            actual,
            expectation->test_file,
            expectation->test_line,
            test_reporter);
    }
}

static CgreenValue stored_result_or_default_for(CgreenVector* constraints) {
    int i;
    for (i = 0; i < cgreen_vector_size(constraints); i++) {
        Constraint *constraint = (Constraint *)cgreen_vector_get(constraints, i);

        if (constraint->type == RETURN_VALUE) {
            return constraint->expected_value;
        }
    }

    return (CgreenValue){INTEGER, {0}};
}

static bool is_always_call(RecordedExpectation* expectation) {
    return expectation->time_to_live == UNLIMITED_TIME_TO_LIVE;
}

static bool have_always_expectation_for(const char* function) {
    int i;
    for (i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
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

static bool is_never_call(RecordedExpectation* expectation) {
    return expectation->time_to_live == -UNLIMITED_TIME_TO_LIVE;
}

static bool have_never_call_expectation_for(const char* function) {
    int i;
    for (i = 0; i < cgreen_vector_size(global_expectation_queue); i++) {
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

/* vim: set ts=4 sw=4 et cindent: */
