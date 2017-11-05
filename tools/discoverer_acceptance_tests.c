#include <cgreen/cgreen.h>

#include "discoverer.h"

#include <stdio.h>

Describe(Discoverer);
BeforeEach(Discoverer) {}
AfterEach(Discoverer) {}

static bool verbose = false;

static int count_tests_in(const char *filename) {
    char command[1000];
    sprintf(command, "/usr/bin/nm '%s'", filename);
    FILE *nm_output = popen(command, "r");
    char line[10000];
    int count = 0;
    while (fgets(line, sizeof(line), nm_output) != 0) {
        if (strstr(line, CGREEN_SPEC_PREFIX CGREEN_SEPARATOR) != NULL)
            count++;
    }
    return count;
}

Ensure(Discoverer, reads_a_library_and_finds_the_tests) {
    char filename[] = "libdiscoverer_unit_tests.so";
    CgreenVector *tests = discover_tests_in(filename, verbose);
    assert_that(cgreen_vector_size(tests), is_equal_to(count_tests_in(filename)));
}

TestSuite *discoverer_acceptance_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Discoverer, reads_a_library_and_finds_the_tests);
    return suite;
}
