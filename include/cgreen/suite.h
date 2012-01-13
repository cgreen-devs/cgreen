#ifndef SUITE_HEADER
#define SUITE_HEADER

#include <cgreen/reporter.h>
#include <cgreen/unit.h>
#include <stdbool.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

enum {test_function, test_suite};

typedef struct TestSuite_ TestSuite;

typedef struct {
    int type;
    union {
        CgreenTest *test;
        TestSuite *suite;
    } sPtr;
    const char *name;
} UnitTest;

struct TestSuite_ {
	const char *name;
	UnitTest *tests;
	void (*setup)();
	void (*teardown)();
	int size;
};

void do_nothing(void);

#define create_test_suite() create_named_test_suite(__func__)
#define add_test(suite, test) add_test_(suite, #test, &spec_name(default, test))
#define add_test_with_context(suite, context, test) add_test_(suite, #test, &spec_name(context, test))
#define add_tests(suite, ...) add_tests_(suite, #__VA_ARGS__, (CgreenTest *)__VA_ARGS__ +0)
#define add_suite(owner, suite) add_suite_(owner, #suite, suite)

TestSuite *create_named_test_suite(const char *name);
void destroy_test_suite(TestSuite *suite);
void add_test_(TestSuite *suite, const char *name, CgreenTest *test);
void add_tests_(TestSuite *suite, const char *names, ...);
void add_suite_(TestSuite *owner, const char *name, TestSuite *suite);
void set_setup(TestSuite *suite, void (*set_up)());
void set_teardown(TestSuite *suite, void (*tear_down)());
int count_tests(TestSuite *suite);
bool has_test(TestSuite *suite, const char *name);
bool has_setup(TestSuite *suite);
bool has_teardown(TestSuite *suite);

#ifdef __cplusplus
    }
}
#endif


#endif
