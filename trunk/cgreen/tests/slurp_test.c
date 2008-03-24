#include "../cgreen.h"
#include "../slurp.h"
#include <stdlib.h>
#include <string.h>

static char *garbage = NULL;

static void clean_up() {
    free(garbage);
}

Ensure missing_file_gives_null() {
    assert_equal(garbage = slurp("tests/samples/not_there", 1024),
                 NULL);
}

Ensure whole_file_can_be_read() {
    assert_string_equal(garbage = slurp("tests/samples/some_file", 1024),
                        "Some stuff\n");
}

Ensure whole_file_can_be_read_in_multiple_small_blocks() {
    assert_string_equal(garbage = slurp("tests/samples/some_file", 1),
                        "Some stuff\n");
}

TestSuite *slurp_tests() {
    TestSuite *suite = create_test_suite();
    teardown(suite, clean_up);
    add_test(suite, missing_file_gives_null);
    add_test(suite, whole_file_can_be_read);
    add_test(suite, whole_file_can_be_read_in_multiple_small_blocks);
    return suite;
}
