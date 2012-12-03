#include <cgreen/assertions.h>
#include <cgreen/mocks.h>
#include <cgreen/reporter.h>
#include <cgreen/runner.h>
#include <cgreen/suite.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef WIN32
#include <sys/wait.h>
#endif

#ifdef __cplusplus
#include <stdexcept>

namespace cgreen {
#endif

static const char* CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE = "CGREEN_PER_TEST_TIMEOUT";

typedef void (*sighandler_t)(int);

static void clean_up_test_run(TestSuite *suite, TestReporter *reporter);
static void run_every_test(TestSuite *suite, TestReporter *reporter);
static void run_named_test(TestSuite *suite, const char *name, TestReporter *reporter);

static void run_test_in_the_current_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter);
static void run_test_in_its_own_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter);


static int in_child_process(void);
static void wait_for_child_process(void);
static void ignore_ctrl_c(void);
static void allow_ctrl_c(void);
static void stop(void);
static int per_test_timeout_defined(void);
static int per_test_timeout_value(void);
static void validate_per_test_timeout_value(void);
static void run_the_test_code(TestSuite *suite, CgreenTest *test, TestReporter *reporter);
static void die(const char *message, ...);


int run_test_suite(TestSuite *suite, TestReporter *reporter) {
	if (per_test_timeout_defined()) {
		validate_per_test_timeout_value();
	}

	setup_reporting(reporter);
	run_every_test(suite, reporter);
	int success = (reporter->failures == 0);
	clean_up_test_run(suite, reporter);
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

int run_single_test(TestSuite *suite, const char *name, TestReporter *reporter) {
	if (per_test_timeout_defined()) {
		validate_per_test_timeout_value();
	}

	setup_reporting(reporter);
	run_named_test(suite, name, reporter);
	int success = (reporter->failures == 0);
	clean_up_test_run(suite, reporter);
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void clean_up_test_run(TestSuite *suite, TestReporter *reporter) {
    (*reporter->destroy)(reporter);
	destroy_test_suite(suite);
}

static void run_every_test(TestSuite *suite, TestReporter *reporter) {
	(*reporter->start_suite)(reporter, suite->name, count_tests(suite));
	int i;
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
#ifndef WIN32
            run_test_in_its_own_process(suite, suite->tests[i].Runnable.test, reporter);
#else
            run_test_in_the_current_process(suite, suite->tests[i].Runnable.test, reporter);
#endif
        } else {
            (*suite->setup)();
            run_every_test(suite->tests[i].Runnable.suite, reporter);
            (*suite->teardown)();
        }
    }
    send_reporter_completion_notification(reporter);
    (*reporter->finish_suite)(reporter, suite->filename, suite->line);
}

static void run_named_test(TestSuite *suite, const char *name, TestReporter *reporter) {
    (*reporter->start_suite)(reporter, suite->name, count_tests(suite));
    int i;
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (strcmp(suite->tests[i].name, name) == 0) {
                run_test_in_the_current_process(suite, suite->tests[i].Runnable.test, reporter);
            }
        } else if (has_test(suite->tests[i].Runnable.suite, name)) {
            (*suite->setup)();
            run_named_test(suite->tests[i].Runnable.suite, name, reporter);
            (*suite->teardown)();
        }
    }
    send_reporter_completion_notification(reporter);
    (*reporter->finish_suite)(reporter, suite->filename, suite->line);
}


static void run_test_in_the_current_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    (*reporter->start_test)(reporter, test->name);
    run_the_test_code(suite, test, reporter);
    send_reporter_completion_notification(reporter);
    (*reporter->finish_test)(reporter, test->filename, test->line);
}

#ifndef WIN32
static void run_test_in_its_own_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    (*reporter->start_test)(reporter, test->name);
    if (in_child_process()) {
        run_the_test_code(suite, test, reporter);
        send_reporter_completion_notification(reporter);
        stop();
    } else {
        wait_for_child_process();
        (*reporter->finish_test)(reporter, test->filename, test->line);
    }
}

static int in_child_process() {
    fflush(NULL);               /* Flush all buffers before forking */
    pid_t child = fork();
    if (child < 0) {
        die("Could not fork process\n");
    }
    return ! child;
}

static void wait_for_child_process() {
    int status;
    ignore_ctrl_c();
    wait(&status);
    allow_ctrl_c();
}
#endif

static void ignore_ctrl_c() {
    signal(SIGINT, SIG_IGN);
}

static void allow_ctrl_c() {
    signal(SIGINT, SIG_DFL);
}

static void stop() {
    _exit(EXIT_SUCCESS);
}

static int per_test_timeout_defined() {
	return getenv(CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE) != NULL;
}

static int per_test_timeout_value() {
	if (!per_test_timeout_defined()) {
		die("attempt to fetch undefined value for %s\n", CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE);
	}

	char* timeout_string = getenv(CGREEN_PER_TEST_TIMEOUT_ENVIRONMENT_VARIABLE);
	int timeout_value = atoi(timeout_string);

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
    va_list no_arguments;
	char message[255];
	TestReporter *reporter = get_test_reporter();

    try {
#endif
    	spec->context->setup();
#ifdef __cplusplus
    } catch(std::exception& exception) {
    	snprintf(message, sizeof(message) - 1, "an exception was thrown during setup: [%s]", exception.what());
    	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::exception* exception) {
    	snprintf(message, sizeof(message) - 1, "an exception was thrown during setup: [%s]", exception->what());
    	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::string& exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during setup: [%s]", exception_message.c_str());

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::string *exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during setup: [%s]", exception_message->c_str());

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(const char *exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during setup: [%s]", exception_message);

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    }
#endif

}

static void run_teardown_for(CgreenTest *spec) {
#ifdef __cplusplus
    va_list no_arguments;
	char message[255];
	TestReporter *reporter = get_test_reporter();

    try {
#endif
    	spec->context->teardown();
#ifdef __cplusplus
    } catch(std::exception& exception) {
    	snprintf(message, sizeof(message) - 1, "an exception was thrown during teardown: [%s]", exception.what());
    	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::exception* exception) {
    	snprintf(message, sizeof(message) - 1, "an exception was thrown during teardown: [%s]", exception->what());
    	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::string& exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during teardown: [%s]", exception_message.c_str());

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::string *exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during teardown: [%s]", exception_message->c_str());

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(const char *exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during teardown: [%s]", exception_message);

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    }
#endif

}


static void run(CgreenTest *spec) {
#ifdef __cplusplus
    va_list no_arguments;
    char message[255];
    TestReporter *reporter = get_test_reporter();

    try {
#endif
    	spec->run();
#ifdef __cplusplus
    } catch(std::exception& exception) {
    	snprintf(message, sizeof(message) - 1, "an exception was thrown during test: [%s]", exception.what());
    	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::exception* exception) {
    	snprintf(message, sizeof(message) - 1, "an exception was thrown during test: [%s]", exception->what());
    	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::string& exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during test: [%s]", exception_message.c_str());

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(std::string *exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during test: [%s]", exception_message->c_str());

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    } catch(const char *exception_message) {
        	snprintf(message, sizeof(message) - 1, "an exception was thrown during test: [%s]", exception_message);

        	reporter->show_fail(reporter, spec->filename, spec->line, message, no_arguments);
    }
#endif

}

static void run_the_test_code(TestSuite *suite, CgreenTest *spec, TestReporter *reporter) {
    significant_figures_for_assert_double_are(8);
    clear_mocks();

    if (per_test_timeout_defined()) {
    	validate_per_test_timeout_value();
#ifndef WIN32
    	die_in(per_test_timeout_value());
#else
        va_list no_arguments;
        reporter->show_fail(reporter, spec->filename, spec->line, "per-test timeouts are currently not supported on win32 platforms", no_arguments);
#endif
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
    if (suite->teardown != &do_nothing) {
        (*suite->teardown)();
    } else {
        if (spec->context->teardown != NULL) {
        	run_teardown_for(spec);
        }
    }

    tally_mocks(reporter);
}

static void die(const char *message, ...) {
	va_list arguments;
	va_start(arguments, message);
	vprintf(message, arguments);
	va_end(arguments);
	exit(EXIT_FAILURE);
}

#ifndef WIN32
void die_in(unsigned int seconds) {
    sighandler_t signal_result = signal(SIGALRM, (sighandler_t)&stop);
    if (SIG_ERR == signal_result) {
        fprintf(stderr, "could not set alarm signal hander\n");
        return;
    }

    alarm(seconds);
}
#endif


#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
