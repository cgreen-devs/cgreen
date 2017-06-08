#include <cgreen/assertions.h>
#include <cgreen/mocks.h>
#include <cgreen/reporter.h>
#include <cgreen/suite.h>
#include <cgreen/internal/runner_platform.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <cgreen/internal/cgreen_time.h>

#include "runner.h"

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__


static const char* CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE = "CGREEN_PER_TEST_TIMEOUT";

static void run_every_test(TestSuite *suite, TestReporter *reporter);
static void run_named_test(TestSuite *suite, const char *name, TestReporter *reporter);

static void run_test_in_the_current_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter);

static int per_test_timeout_defined(void);
static int per_test_timeout_value(void);
static void validate_per_test_timeout_value(void);

int run_test_suite(TestSuite *suite, TestReporter *reporter) {
    int success;
    if (per_test_timeout_defined()) {
        validate_per_test_timeout_value();
    }

    setup_reporting(reporter);
    run_every_test(suite, reporter);
    success = (reporter->total_failures == 0) && (reporter->total_exceptions==0);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

int run_single_test(TestSuite *suite, const char *name, TestReporter *reporter) {
    int success;
    if (per_test_timeout_defined()) {
        validate_per_test_timeout_value();
    }

    setup_reporting(reporter);
    run_named_test(suite, name, reporter);
    success = (reporter->total_failures == 0);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void run_every_test(TestSuite *suite, TestReporter *reporter) {
    int i;

    run_specified_test_if_child(suite, reporter);

    uint32_t total_test_starting_milliseconds = cgreen_time_get_current_milliseconds();
    (*reporter->start_suite)(reporter, suite->name, count_tests(suite));

    // Run sub-suites first
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type != test_function) {
            (*suite->setup)();
            run_every_test(suite->tests[i].Runnable.suite, reporter);
            (*suite->teardown)();
        }
    }

    // Reset counters for top-level tests
    reporter->passes = 0;
    reporter->failures = 0;
    reporter->skips = 0;
    reporter->exceptions = 0;

    uint32_t test_starting_milliseconds = cgreen_time_get_current_milliseconds();

    // Run top-level tests
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (getenv("CGREEN_NO_FORK") == NULL)
                run_test_in_its_own_process(suite, suite->tests[i].Runnable.test, reporter);
            else
                run_test_in_the_current_process(suite, suite->tests[i].Runnable.test, reporter);
        }
    }

    reporter->duration = cgreen_time_duration_in_milliseconds(test_starting_milliseconds,
                                                                  cgreen_time_get_current_milliseconds());
    reporter->total_duration = cgreen_time_duration_in_milliseconds(total_test_starting_milliseconds,
                                                                  cgreen_time_get_current_milliseconds());
    send_reporter_completion_notification(reporter);
    (*reporter->finish_suite)(reporter, suite->filename, suite->line);
}

static void run_named_test(TestSuite *suite, const char *name, TestReporter *reporter) {
    int i;

    uint32_t total_test_starting_milliseconds = cgreen_time_get_current_milliseconds();

    (*reporter->start_suite)(reporter, suite->name, count_tests(suite));
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type != test_function &&
                has_test(suite->tests[i].Runnable.suite, name)) {
            (*suite->setup)();
            run_named_test(suite->tests[i].Runnable.suite, name, reporter);
            (*suite->teardown)();
        }
    }

    // Reset counters for top-level tests
    reporter->passes = 0;
    reporter->failures = 0;
    reporter->skips = 0;
    reporter->exceptions = 0;

    uint32_t test_starting_milliseconds = cgreen_time_get_current_milliseconds();

    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (strcmp(suite->tests[i].name, name) == 0) {
                run_test_in_the_current_process(suite, suite->tests[i].Runnable.test, reporter);
            }
        }
    }

    reporter->duration = cgreen_time_duration_in_milliseconds(test_starting_milliseconds,
                                                                  cgreen_time_get_current_milliseconds());
    reporter->total_duration = cgreen_time_duration_in_milliseconds(total_test_starting_milliseconds,
                                                                  cgreen_time_get_current_milliseconds());

    send_reporter_completion_notification(reporter);
    (*reporter->finish_suite)(reporter, suite->filename, suite->line);
}


static void run_test_in_the_current_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    uint32_t test_starting_milliseconds = cgreen_time_get_current_milliseconds();

    (*reporter->start_test)(reporter, test->name);
    if (test->skip) {
        send_reporter_skipped_notification(reporter);
    } else {
        run_the_test_code(suite, test, reporter);
        reporter->duration = cgreen_time_duration_in_milliseconds(test_starting_milliseconds,
                                                             cgreen_time_get_current_milliseconds());

        send_reporter_completion_notification(reporter);
    }
    (*reporter->finish_test)(reporter, test->filename, test->line, NULL);
}

static int per_test_timeout_defined() {
    return getenv(CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE) != NULL;
}

static int per_test_timeout_value() {
    char* timeout_string;
    int timeout_value;

    if (!per_test_timeout_defined()) {
        die("attempt to fetch undefined value for %s\n", CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE);
    }

    timeout_string = getenv(CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE);
    timeout_value = atoi(timeout_string);

    return timeout_value;
}

static void validate_per_test_timeout_value() {
    int timeout = per_test_timeout_value();

    if (timeout <= 0) {
        die("invalid value for %s environment variable: %d\n", CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE, timeout);
    }
}

static void run_setup_for(CgreenTest *spec) {
#ifdef __cplusplus
    std::string message = "an exception was thrown during setup: ";
    try {
#endif
        spec->context->setup();
#ifdef __cplusplus
        return;
    } catch(const std::exception& exception) {
        message += '[';
        message += exception.what();
        message += ']';
    } catch(const std::exception* exception) {
        message += '[';
        message += exception->what();
        message += ']';
    } catch(const std::string& exception_message) {
        message += '[';
        message += exception_message;
        message += ']';
    } catch(const std::string *exception_message) {
        message += '[';
        message += *exception_message;
        message += ']';
    } catch(const char *exception_message) {
        message += '[';
        message += exception_message;
        message += ']';
    } catch (...) {
        message += "unknown exception type";
    }
    va_list no_arguments;
    memset(&no_arguments, 0, sizeof(va_list));
    TestReporter *reporter = get_test_reporter();
    reporter->show_incomplete(reporter, spec->filename, spec->line, message.c_str(), no_arguments);
    send_reporter_exception_notification(reporter);
#endif
}

static void run_teardown_for(CgreenTest *spec) {
#ifdef __cplusplus
    std::string message = "an exception was thrown during teardown: ";
    try {
#endif
        spec->context->teardown();
#ifdef __cplusplus
        return;
    } catch(const std::exception& exception) {
        message += '[';
        message += exception.what();
        message += ']';
    } catch(const std::exception* exception) {
        message += '[';
        message += exception->what();
        message += ']';
    } catch(const std::string& exception_message) {
        message += '[';
        message += exception_message;
        message += ']';
    } catch(const std::string *exception_message) {
        message += '[';
        message += *exception_message;
        message += ']';
    } catch(const char *exception_message) {
        message += '[';
        message += exception_message;
        message += ']';
    } catch (...) {
        message += "unknown exception type";
    }
    va_list no_arguments;
    memset(&no_arguments, 0, sizeof(va_list));
    TestReporter *reporter = get_test_reporter();
    reporter->show_incomplete(reporter, spec->filename, spec->line, message.c_str(), no_arguments);
    send_reporter_exception_notification(reporter);
#endif
}

/**
   run()

   N.B. Although this is neither an API or public function, it is
   documented as a good place to put a breakpoint. Do not change the
   name or semantics of this function, it should continue to be very
   close to the test code. */
static void run(CgreenTest *spec) {
#ifdef __cplusplus
    std::string message = "an exception was thrown during test: ";
    try {
#endif
        spec->run();
#ifdef __cplusplus
        return;
    } catch(const std::exception& exception) {
        message += '[';
        message += exception.what();
        message += ']';
    } catch(const std::exception* exception) {
        message += '[';
        message += exception->what();
        message += ']';
    } catch(const std::string& exception_message) {
        message += '[';
        message += exception_message;
        message += ']';
    } catch(const std::string *exception_message) {
        message += '[';
        message += *exception_message;
        message += ']';
    } catch(const char *exception_message) {
        message += '[';
        message += exception_message;
        message += ']';
    } catch (...) {
        message += "unknown exception type";
    }
    va_list no_arguments;
    memset(&no_arguments, 0, sizeof(va_list));
    TestReporter *reporter = get_test_reporter();
    reporter->show_incomplete(reporter, spec->filename, spec->line, message.c_str(), no_arguments);
    send_reporter_exception_notification(reporter);
#endif
}

void run_the_test_code(TestSuite *suite, CgreenTest *spec, TestReporter *reporter) {
    significant_figures_for_assert_double_are(8);
    clear_mocks();

    if (per_test_timeout_defined()) {
        validate_per_test_timeout_value();

        die_in(per_test_timeout_value());
    }

    // for historical reasons the suite can have a setup
    if(has_setup(suite)) {
        (*suite->setup)();
    } else {
        if (spec->context->setup != NULL) {
            run_setup_for(spec);
        }
    }

    run(spec);
    // for historical reasons the suite can have a teardown
    if (has_teardown(suite)) {
        (*suite->teardown)();
    } else {
        if (spec->context->teardown != NULL) {
            run_teardown_for(spec);
        }
    }

    tally_mocks(reporter);
}

void die(const char *message, ...) {
    va_list arguments;
    va_start(arguments, message);
    vprintf(message, arguments);
    va_end(arguments);
    exit(EXIT_FAILURE);
}

/* vim: set ts=4 sw=4 et cindent: */
