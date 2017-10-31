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

static void expect_open_file(const char *filename, void *result) {
    expect(open_file, when(filename,
                           is_equal_to_string(filename)),
           will_return(result));
}



Ensure(Discoverer, should_find_no_tests_in_non_existing_file) {
    expect_open_file("non-existing-file", NULL);

    CgreenVector *tests = discover_tests_in("non-existing-file");

    assert_that(tests, is_null);
}


static void expect_open_process(const char *partial_command, void *result) {
    expect(open_process, when(command, contains_string(partial_command)),
           will_return(result));
}


Ensure(Discoverer, should_find_no_tests_in_existing_empty_file) {
    expect_open_file("empty-file", (void *)1);
    expect_open_process("nm ", (void *)2);
    expect(read_line, when(file, is_equal_to(2)),
           will_return(EOF));     /* End of input */

    CgreenVector *tests = discover_tests_in("empty-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}

static void expect_read_line_from(int file_id, char line[]) {
    expect(read_line, when(file, is_equal_to(file_id)),
           will_set_contents_of_parameter(buffer, line, strlen(line)+1),
           will_return(strlen(line)>0?strlen(line)+1:EOF));
}


Ensure(Discoverer, should_find_one_test_in_file_with_one_line_containing_testname_pattern) {
    char line[] = "0000000000202160 D CgreenSpec__Discoverer__should_find_no_tests_in_existing_empty_file__";

    expect_open_file("some-file", (void *)1);
    expect_open_process("nm ", (void *)2);
    expect_read_line_from(2, line);
    expect_read_line_from(2, "");

    CgreenVector *tests = discover_tests_in("some-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}

Ensure(Discoverer, should_find_two_test_in_two_line_file_with_two_lines_containing_testname_pattern) {
    char line1[] = "0000000000202160 D CgreenSpec__Discoverer__test1__";
    char line2[] = "0000000000202160 D CgreenSpec__Discoverer__test2__";

    expect_open_file("some-file", (void *)1);
    expect_open_process("nm ", (void *)2);
    expect_read_line_from(2, line1);
    expect_read_line_from(2, line2);
    expect_read_line_from(2, "");

    CgreenVector *tests = discover_tests_in("some-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(2));
}

Ensure(Discoverer, should_find_one_test_in_two_line_file_with_one_line_containing_testname_pattern) {
    char line1[] = "0000000000202160 D CgreenSpec__Discoverer__test1__";
    char line2[] = "0000000000202160 D ID";

    expect_open_file("some-file", (void *)1);
    expect_open_process("nm ", (void *)2);
    expect_read_line_from(2, line1);
    expect_read_line_from(2, line2);
    expect_read_line_from(2, "");

    CgreenVector *tests = discover_tests_in("some-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}

Ensure(Discoverer, should_find_no_test_in_file_with_no_definiton) {
    char line[] = "0000000000202160 U CgreenSpec__Discoverer__test1__";

    expect_open_file("some-file", (void *)1);
    expect_open_process("nm ", (void *)2);
    expect_read_line_from(2, line);
    expect_read_line_from(2, "");

    CgreenVector *tests = discover_tests_in("some-file");

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}
