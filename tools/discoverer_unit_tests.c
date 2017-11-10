#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "discoverer.h"
#include "test_item.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "io.h"
#include "io.mocks"


Describe(Discoverer);
BeforeEach(Discoverer) {}
AfterEach(Discoverer) {}


static bool verbose = false;


/* Helper functions */
static void expect_open_file(const char *filename, void *result) {
    expect(open_file, when(filename,
                           is_equal_to_string(filename)),
           will_return(result));
}

static void expect_open_process(const char *partial_command, void *result) {
    expect(open_process, when(command, contains_string(partial_command)),
           will_return(result));
}

static void given_a_file_with_no_lines(const char *filename) {
    expect_open_file(filename, (void *)1);
    expect(close_file, when(file, is_equal_to(1)));
    expect_open_process("nm ", (void *)2);
    expect(read_line, when(file, is_equal_to(2)),
           will_return(EOF));     /* End of input */
    expect(close_process, when(file, is_equal_to(2)));
}

static void expect_read_line_from(int file_id, const char *line) {
    if (line == NULL)
        expect(read_line, when(file, is_equal_to(file_id)),
               will_return(EOF));
    else
        expect(read_line, when(file, is_equal_to(file_id)),
               will_set_contents_of_parameter(buffer, line, strlen(line)+1),
               will_return(strlen(line)+1));
}

static void given_a_file_with_two_lines(const char *filename, const char *line1, const char *line2) {
    static char command[100];
    expect_open_file(filename, (void *)1);
    expect(close_file, when(file, is_equal_to(1)));
    sprintf(command, "nm %s", filename);
    expect_open_process(command, (void *)2);
    expect_read_line_from(2, line1);
    expect_read_line_from(2, line2);
    expect_read_line_from(2, NULL);
    expect(close_process, when(file, is_equal_to(2)));
}

static void given_a_file_with_one_line(const char *filename, const char *line) {
    expect_open_file(filename, (void *)1);
    expect(close_file, when(file, is_equal_to(1)));
    expect_open_process("nm ", (void *)2);
    expect_read_line_from(2, line);
    expect_read_line_from(2, NULL);
    expect(close_process, when(file, is_equal_to(2)));
}



/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_in_non_existing_file) {
    expect_open_file("non-existing-file", NULL);

    CgreenVector *tests = discover_tests_in("non-existing-file", verbose);

    assert_that(tests, is_null);
}


/*======================================================================*/
Ensure(Discoverer, should_find_no_tests_in_existing_empty_file) {
    given_a_file_with_no_lines("empty-file");

    CgreenVector *tests = discover_tests_in("empty-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}


/*======================================================================*/
Ensure(Discoverer, should_find_one_test_in_file_with_one_line_containing_testname_pattern) {
    given_a_file_with_one_line("some-file",
                               "0000000000202160 D CgreenSpec__A_context__a_test__\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}


/*======================================================================*/
Ensure(Discoverer, should_find_two_test_in_two_line_file_with_two_lines_containing_testname_pattern) {
    given_a_file_with_two_lines("some-file",
                                "0000000000202160 D CgreenSpec__Context1__test1__\n",
                                "0000000000202160 D CgreenSpec__Context2__test2__\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(2));
}


/*======================================================================*/
Ensure(Discoverer, should_find_one_test_in_two_line_file_with_one_line_containing_testname_pattern) {
    given_a_file_with_two_lines("some-file",
                                "0000000000202160 D CgreenSpec__Discoverer__test1__\n",
                                "0000000000202160 D ID\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(1));
}


/*======================================================================*/
Ensure(Discoverer, should_find_no_test_in_file_with_no_definiton) {
    given_a_file_with_one_line("some-file",
                               "0000000000202160 U CgreenSpec__Discoverer__test1__\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}


/*======================================================================*/
Ensure(Discoverer, should_strip_newline_at_end_of_specification_name) {
    given_a_file_with_one_line("some-file",
                               "0000000000202160 U CgreenSpec__Discoverer__test1__\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(0));
}


/*======================================================================*/
Ensure(Discoverer, should_return_valid_test_item_for_a_line_containing_testname_pattern) {
    given_a_file_with_one_line("some-file",
                               "0000000000202160 D CgreenSpec__Context1__test_1__\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    assert_that(cgreen_vector_size(tests), is_equal_to(1));

    TestItem *test_item = (TestItem*)cgreen_vector_get(tests, 0);
    assert_that(test_item->specification_name, is_equal_to_string("CgreenSpec__Context1__test_1__"));
    assert_that(test_item->context_name, is_equal_to_string("Context1"));
    assert_that(test_item->test_name, is_equal_to_string("test_1"));
}

/*======================================================================*/
Ensure(Discoverer, can_handle_truncated_testnames) {
    given_a_file_with_one_line("some-file", "0000000000202160 D CgreenSpec__Context1__a\n");

    CgreenVector *tests = discover_tests_in("some-file", verbose);

    TestItem *test_item = (TestItem*)cgreen_vector_get(tests, 0);
    assert_that(test_item->test_name, is_equal_to_string("a"));
}
