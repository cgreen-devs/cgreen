#include "config.h"
#include <cgreen/cgreen.h>
#include <cgreen/slurp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void assert_slurped(char *path, int gulp, const char *expected_contents);

Ensure(missing_file_gives_null) {
    assert_equal(slurp("not_there", 1024), NULL);
}

Ensure(whole_file_can_be_read) {
    assert_slurped(BINARYDIR "/tests/some_file", 1024, "Some stuff");
}

Ensure(whole_file_can_be_read_in_multiple_small_blocks) {
    assert_slurped(BINARYDIR "/tests/some_file", 1, "Some stuff");
}

static void assert_slurped(char *path, int gulp, const char *expected_contents) {
    char *buffer;
    buffer = slurp(path, gulp);
    assert_string_equal(buffer, expected_contents);
    free(buffer);
}

TestSuite *slurp_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, missing_file_gives_null);
    add_test(suite, whole_file_can_be_read);
    add_test(suite, whole_file_can_be_read_in_multiple_small_blocks);
    return suite;
}
