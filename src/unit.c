#include <cgreen/unit.h>
#include <cgreen/reporter.h>
#include <cgreen/mocks.h>
#include <cgreen/parameters.h>
#include <cgreen/assertions.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

enum {test_function, test_suite};

typedef void (*sighandler_t)(int);

typedef struct {
    int type;
    union {
        void (*test)();
        TestSuite *suite;
    };
    char *name;
} UnitTest;

struct TestSuite_ {
	const char *name;
    UnitTest *tests;
    void (*setup)();
    void (*teardown)();
    int size;
};

static void clean_up_test_run(TestSuite *suite, TestReporter *reporter);
static void run_every_test(TestSuite *suite, TestReporter *reporter);
static void run_named_test(TestSuite *suite, char *name, TestReporter *reporter);
static int has_test(TestSuite *suite, char *name);
static void run_test_in_the_current_process(TestSuite *suite, UnitTest *test, TestReporter *reporter);
static void run_test_in_its_own_process(TestSuite *suite, UnitTest *test, TestReporter *reporter);
static int in_child_process();
static void wait_for_child_process();
static void ignore_ctrl_c();
static void allow_ctrl_c();
static void stop();
static void run_the_test_code(TestSuite *suite, UnitTest *test, TestReporter *reporter);
static void tally_counter(const char *file, int line, int expected, int actual, void *abstract_reporter);
static void die(const char *message, ...);
static void do_nothing();

TestSuite *create_named_test_suite(const char *name) {
    TestSuite *suite = (TestSuite *)malloc(sizeof(TestSuite));
	suite->name = name;
    suite->tests = NULL;
    suite->setup = &do_nothing;
    suite->teardown = &do_nothing;
    suite->size = 0;
    return suite;
}

void destroy_test_suite(TestSuite *suite) {
    free(suite->tests);
    free(suite);
}

void add_test_(TestSuite *suite, char *name, CgreenTest *test) {
    suite->size++;
    suite->tests = (UnitTest *)realloc(suite->tests, sizeof(UnitTest) * suite->size);
    suite->tests[suite->size - 1].type = test_function;
    suite->tests[suite->size - 1].name = name;
    suite->tests[suite->size - 1].test = test;
}

void add_tests_(TestSuite *suite, const char *names, ...) {
    CgreenVector *test_names = create_vector_of_names(names);
    int i;
    va_list tests;
    va_start(tests, names);
    for (i = 0; i < cgreen_vector_size(test_names); i++) {
        add_test_(suite, (char *)cgreen_vector_get(test_names, i), va_arg(tests, CgreenTest *));
    }
    va_end(tests);
    destroy_cgreen_vector(test_names);
}

void add_suite_(TestSuite *owner, char *name, TestSuite *suite) {
    owner->size++;
    owner->tests = (UnitTest *)realloc(owner->tests, sizeof(UnitTest) * owner->size);
    owner->tests[owner->size - 1].type = test_suite;
    owner->tests[owner->size - 1].name = name;
    owner->tests[owner->size - 1].suite = suite;
}

void setup_(TestSuite *suite, void (*setup)()) {
    suite->setup = setup;
}

void teardown_(TestSuite *suite, void (*teardown)()) {
    suite->teardown = teardown;
}

void die_in(unsigned int seconds) {
    signal(SIGALRM, (sighandler_t)&stop);
    alarm(seconds);
}

int count_tests(TestSuite *suite) {
	int count = 0;
	int i;
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            count++;
        } else {
            count += count_tests(suite->tests[i].suite);
        }
    }
    return count;
}

int run_test_suite(TestSuite *suite, TestReporter *reporter) {
	setup_reporting(reporter);
	run_every_test(suite, reporter);
	int success = (reporter->failures == 0);
	clean_up_test_run(suite, reporter);
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

int run_single_test(TestSuite *suite, char *name, TestReporter *reporter) {
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
            run_test_in_its_own_process(suite, &(suite->tests[i]), reporter);
        } else {
            (*suite->setup)();
            run_every_test(suite->tests[i].suite, reporter);
            (*suite->teardown)();
        }
    }
    send_reporter_completion_notification(reporter);
	(*reporter->finish_suite)(reporter, suite->name);
}

static void run_named_test(TestSuite *suite, char *name, TestReporter *reporter) {
	(*reporter->start_suite)(reporter, suite->name, count_tests(suite));
	int i;
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (strcmp(suite->tests[i].name, name) == 0) {
                run_test_in_the_current_process(suite, &(suite->tests[i]), reporter);
            }
        } else if (has_test(suite->tests[i].suite, name)) {
            (*suite->setup)();
            run_named_test(suite->tests[i].suite, name, reporter);
            (*suite->teardown)();
        }
    }
    send_reporter_completion_notification(reporter);
	(*reporter->finish_suite)(reporter, suite->name);
}

static int has_test(TestSuite *suite, char *name) {
	int i;
	for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (strcmp(suite->tests[i].name, name) == 0) {
                return 1;
            }
        } else if (has_test(suite->tests[i].suite, name)) {
            return 1;
        }
	}
	return 0;
}

static void run_test_in_the_current_process(TestSuite *suite, UnitTest *test, TestReporter *reporter) {
	(*reporter->start_test)(reporter, test->name);
	run_the_test_code(suite, test, reporter);
    send_reporter_completion_notification(reporter);
	(*reporter->finish_test)(reporter, test->name);
}

static void run_test_in_its_own_process(TestSuite *suite, UnitTest *test, TestReporter *reporter) {
	(*reporter->start_test)(reporter, test->name);
    if (in_child_process()) {
        run_the_test_code(suite, test, reporter);
        send_reporter_completion_notification(reporter);
        stop();
    } else {
        wait_for_child_process();
        (*reporter->finish_test)(reporter, test->name);
    }
}

static int in_child_process() {
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

static void ignore_ctrl_c() {
    signal(SIGINT, SIG_IGN);
}

static void allow_ctrl_c() {
    signal(SIGINT, SIG_DFL);
}

static void stop() {
    exit(EXIT_SUCCESS);
}

static void run_the_test_code(TestSuite *suite, UnitTest *test, TestReporter *reporter) {
    significant_figures_for_assert_double_are(8);
    clear_mocks();
	(*suite->setup)();
    (*test->test)();
	(*suite->teardown)();
	tally_mocks(reporter);
}

static void tally_counter(const char *file, int line, int expected, int actual, void *abstract_reporter) {
    TestReporter *reporter = (TestReporter *)abstract_reporter;
    (*reporter->assert_true)(
            reporter,
            file,
            line,
            (actual == expected),
            "Expected a call count of [%d], but got [%d]",
            expected,
            actual);
}

static void die(const char *message, ...) {
	va_list arguments;
	va_start(arguments, message);
	vprintf(message, arguments);
	va_end(arguments);
	exit(EXIT_FAILURE);
}

static void do_nothing() {
}

/* vim: set ts=4 sw=4 et cindent: */
