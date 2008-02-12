#ifndef UNIT_HEADER
#define UNIT_HEADER

#include "reporter.h"
#include "mocks.h"

#define create_test_suite() create_named_test_suite(__func__)
#define add_test(suite, test) add_test_(suite, #test, &test)
#define add_suite(owner, suite) add_suite_(owner, #suite, suite)
#define setup(suite, function) setup_(suite, &function)
#define teardown(suite, function) teardown_(suite, &function)

#define Ensure static void
typedef struct TestSuite_ TestSuite;

TestSuite *create_named_test_suite(const char *name);
void destroy_test_suite(TestSuite *suite);
void add_test_(TestSuite *suite, char *name, void (*test)(TestReporter *));
void add_suite_(TestSuite *owner, char *name, TestSuite *suite);
void setup_(TestSuite *suite, void (*set_up)());
void teardown_(TestSuite *suite, void (*tear_down)());
void die_in(unsigned int seconds);
int run_test_suite(TestSuite *suite, TestReporter *reporter);
int run_single_test(TestSuite *suite, char *test, TestReporter *reporter);

#endif
