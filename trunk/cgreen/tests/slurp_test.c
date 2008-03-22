#include "../cgreen.h"
#include "../slurp.h"
#include <stdlib.h>
#include <string.h>

Ensure missing_file_gives_null() {
    assert_equal(slurp("tests/samples/not_there"), NULL);
}

Ensure whole_file_can_be_read() {
    assert_string_equal(slurp("tests/samples/some_file"), "Just some stuff\n");
}

TestSuite *slurp_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, missing_file_gives_null);
    //add_test(suite, whole_file_can_be_read);
    return suite;
}
