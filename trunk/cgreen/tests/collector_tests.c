#include <cgreen/cgreen.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

TestSuite *confuse_the_collector(TestSuite *suite, const char *ignored) {
	(void)ignored;
    return suite;
}

TestSuite *collector_tests() {
    TestSuite *suite = create_test_suite();
    add_tests(confuse_the_collector(suite, "\"("));
    return suite;
}
