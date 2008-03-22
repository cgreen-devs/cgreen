#include "../cgreen.h"
#include "../slurp.h"
#include <stdlib.h>
#include <string.h>

Ensure missing_file_gives_null() {
    assert_equal(slurp("tests/samples/not_there", 1024), NULL);
}

Ensure whole_file_can_be_read() {
    assert_string_equal(slurp("tests/samples/some_file", 1024), "Just some stuff\n");
}

Ensure whole_file_can_be_read_in_multiple_reads() {
    assert_string_equal(slurp("tests/samples/some_file", 1), "Just some stuff\n");
}

TestSuite *slurp_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, missing_file_gives_null);
    add_test(suite, whole_file_can_be_read);
    add_test(suite, whole_file_can_be_read_in_multiple_reads);
    return suite;
}
