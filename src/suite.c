#include <cgreen/parameters.h>
#include <cgreen/suite.h>
#include <cgreen/unit.h>
#include <cgreen/vector.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
namespace cgreen {
#endif


CgreenContext defaultContext = {
    /* name     */ "",
    /* filename */ __FILE__,
    /* setup    */ &do_nothing,
    /* teardown */ &do_nothing
};

bool has_test(TestSuite *suite, const char *name) {
	int i;
	for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (strcmp(suite->tests[i].name, name) == 0) {
                return true;
            }
        } else if (has_test(suite->tests[i].Runnable.suite, name)) {
            return true;
        }
	}

	return false;
}

bool has_setup(TestSuite *suite) {
	return (suite->setup != &do_nothing);
}

bool has_teardown(TestSuite *suite) {
	return (suite->teardown != &do_nothing);
}

void do_nothing() {
}

TestSuite *create_named_test_suite_(const char *name, const char *filename, int line) {
    TestSuite *suite = (TestSuite *)malloc(sizeof(TestSuite));
    suite->name = name;
    suite->filename = filename;
    suite->line = line;
    suite->tests = NULL;
    suite->setup = &do_nothing;
    suite->teardown = &do_nothing;
    suite->size = 0;
    return suite;
}

void destroy_test_suite(TestSuite *suiteToDestroy) {
    int i;
    for (i = 0; i < suiteToDestroy->size; i++) {
        UnitTest test = suiteToDestroy->tests[i];
        TestSuite* suite = test.Runnable.suite;
        if (test_suite == test.type && suite != NULL) {
           suiteToDestroy->tests[i].Runnable.suite = NULL;
           destroy_test_suite(suite);
        }
    }

    if (suiteToDestroy->tests != NULL)
        free(suiteToDestroy->tests);

    free(suiteToDestroy);
}

void add_test_(TestSuite *suite, const char *name, CgreenTest *test) {
    suite->size++;
    suite->tests = (UnitTest *)realloc(suite->tests, sizeof(UnitTest) * suite->size);
    suite->tests[suite->size - 1].type = test_function;
    suite->tests[suite->size - 1].name = name;
    suite->tests[suite->size - 1].Runnable.test = test;
}

void add_tests_(TestSuite *suite, const char *names, ...) {
    CgreenVector *test_names = create_vector_of_names(names);
    int i;
    va_list tests;
    va_start(tests, names);
    for (i = 0; i < cgreen_vector_size(test_names); i++) {
        add_test_(suite, (char *)(cgreen_vector_get(test_names, i)), va_arg(tests, CgreenTest *));
    }
    va_end(tests);
    destroy_cgreen_vector(test_names);
}

void add_suite_(TestSuite *owner, const char *name, TestSuite *suite) {
    owner->size++;
    owner->tests = (UnitTest *)realloc(owner->tests, sizeof(UnitTest) * owner->size);
    owner->tests[owner->size - 1].type = test_suite;
    owner->tests[owner->size - 1].name = name;
    owner->tests[owner->size - 1].Runnable.suite = suite;
}

void set_setup(TestSuite *suite, void (*set_up)()) {
    suite->setup = set_up;
}

void set_teardown(TestSuite *suite, void (*tear_down)()) {
    suite->teardown = tear_down;
}

int count_tests(TestSuite *suite) {
    int count = 0;
    int i;
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            count++;
        } else {
            count += count_tests(suite->tests[i].Runnable.suite);
        }
    }
    return count;
}

#ifdef __cplusplus
}
#endif
