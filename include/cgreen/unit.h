#ifndef _UNIT_HEADER_
#define _UNIT_HEADER_

#include "reporter.h"
#include "mocks.h"

#define create_test_suite() create_named_test_suite(__func__)
#define add_test(suite, test) _add_test(suite, #test, &test)
#define add_suite(owner, suite) _add_suite(owner, #suite, suite)
#define setup(suite, function) _setup(suite, &function)
#define teardown(suite, function) _teardown(suite, &function)

typedef struct _TestSuite TestSuite;

TestSuite *create_named_test_suite(const char *name);
void destroy_test_suite(TestSuite *suite);
void _add_test(TestSuite *suite, char *name, void (*test)(TestReporter *));
void _add_suite(TestSuite *owner, char *name, TestSuite *suite);
void _setup(TestSuite *suite, void (*set_up)());
void _teardown(TestSuite *suite, void (*tear_down)());
void die_in(unsigned int seconds);
int run_test_suite(TestSuite *suite, TestReporter *reporter);
int run_single_test(TestSuite *suite, char *test, TestReporter *reporter);

#endif
