#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "discoverer.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "io.h"
#include "io.mocks"


Describe(Discoverer);
BeforeEach(Discoverer) {}
AfterEach(Discoverer) {}


Ensure(Discoverer, should_find_no_tests_in_non_existing_file) {
    expect(open_file, when(filename,
                           is_equal_to_string("non-existing-file")),
           will_return(NULL));

    CgreenVector *tests = discover_tests_in("non-existing-file");

    assert_that(tests, is_null);
}

Ensure(Discoverer, should_find_no_tests_in_existing_empty_file) {
    expect(open_file, when(filename,
                           is_equal_to_string("empty-file")),
           will_return(1));
    expect(open_process, when(command, contains_string("nm ")),
           will_return(2));
    expect(read_line, when(file, is_equal_to(2)),
           will_return(EOF));     /* End of input */

    CgreenVector *tests = discover_tests_in("empty-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

Ensure(Discoverer, should_find_one_test_in_file_with_one_line_containing_testname_pattern) {
    char first_line[] = "alfjslfjlsdjkflj";

    expect(open_file, when(filename,
                           is_equal_to_string("some-file")),
           will_return(1));
    expect(open_process, when(command, contains_string("nm ")),
           will_return(2));
    expect(read_line, when(file, is_equal_to(2)),
           will_set_contents_of_parameter(buffer, first_line, sizeof(first_line)),
           will_return(sizeof(first_line)));
    expect(read_line, when(file, is_equal_to(2)),
           will_return(EOF));

    CgreenVector *tests = discover_tests_in("some-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}
