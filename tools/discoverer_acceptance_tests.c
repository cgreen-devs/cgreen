#include <cgreen/cgreen.h>

#include "discoverer.h"


Describe(Discoverer);
BeforeEach(Discoverer) {}
AfterEach(Discoverer) {}

Ensure(Discoverer, reads_a_library_and_finds_the_tests) {
    CgreenVector *tests = discover_tests_in("libdiscoverer_unit_tests.so");
    assert_that(cgreen_vector_size(tests), is_greater_than(0));
}

TestSuite *discoverer_acceptance_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Discoverer, reads_a_library_and_finds_the_tests);
    return suite;
}
