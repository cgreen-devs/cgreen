#include <cgreen/cgreen.h>
#include <cgreen/vector.h>
#include <stdlib.h>

#include "../src/parameters.h"

#ifdef __cplusplus
using namespace cgreen;
#endif

static CgreenVector *names = NULL;

void destroy_names(void) {
    destroy_cgreen_vector(names);
    names = NULL;
}

Ensure(can_create_vector_of_no_parameters_and_destroy_it) {
    names = create_vector_of_names("");
    assert_that(cgreen_vector_size(names), is_equal_to(0));
}

Ensure(can_read_single_parameter) {
    names = create_vector_of_names("a");
    assert_that(cgreen_vector_size(names), is_equal_to(1));
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
}

Ensure(can_read_two_parameters) {
    names = create_vector_of_names("a, b");
    assert_that(cgreen_vector_size(names), is_equal_to(2));
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_read_three_parameters) {
    names = create_vector_of_names("a, b, c");
    assert_that(cgreen_vector_size(names), is_equal_to(3));
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
    assert_that((const char *)cgreen_vector_get(names, 2), is_equal_to_string("c"));
}

Ensure(can_read_two_parameters_without_spaces) {
    names = create_vector_of_names("a,b");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_read_long_parameters) {
    names = create_vector_of_names("abacus, banana");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("abacus"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("banana"));
}

Ensure(can_read_long_parameters_with_funky_names) {
    names = create_vector_of_names("a_b-c+d, +a-d_c/d");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a_b-c+d"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("+a-d_c/d"));
}

Ensure(can_read_two_parameters_with_varied_whitespace) {
    names = create_vector_of_names("\ra\t,\nb\t\t\t");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_box_double_to_leave_original_name) {
    names = create_vector_of_names("box_double(a)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
}

Ensure(can_strip_two_box_doubles_to_leave_original_names) {
    names = create_vector_of_names("box_double(a), box_double(b)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_argument_and_box_double_to_leave_original_names) {
    names = create_vector_of_names("a, box_double(b)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_box_double_and_argument_to_leave_original_names) {
    names = create_vector_of_names("box_double(a), b");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_d_macro_to_leave_original_name) {
    names = create_vector_of_names("d(a)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
}

Ensure(can_strip_two_d_macros_to_leave_original_names) {
    names = create_vector_of_names("d(a), d(b)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_d_macro_and_box_double_to_leave_original_names) {
    names = create_vector_of_names("d(a), box_double(b)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_box_double_and_d_macro_to_leave_original_names) {
    names = create_vector_of_names("box_double(a), d(b)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
}

Ensure(can_strip_multiple_mixed_parameters_to_leave_original_names) {
    names = create_vector_of_names("box_double(a), d(b), c, d,e,box_double(f),d(g)");
    assert_that((const char *)cgreen_vector_get(names, 0), is_equal_to_string("a"));
    assert_that((const char *)cgreen_vector_get(names, 1), is_equal_to_string("b"));
    assert_that((const char *)cgreen_vector_get(names, 2), is_equal_to_string("c"));
    assert_that((const char *)cgreen_vector_get(names, 3), is_equal_to_string("d"));
    assert_that((const char *)cgreen_vector_get(names, 4), is_equal_to_string("e"));
    assert_that((const char *)cgreen_vector_get(names, 5), is_equal_to_string("f"));
    assert_that((const char *)cgreen_vector_get(names, 6), is_equal_to_string("g"));
}

Ensure(can_create_empty_vector_of_double_markers) {
    CgreenVector *markers = create_vector_of_double_markers_for("");
    assert_that(cgreen_vector_size(markers), is_equal_to(0));
    destroy_cgreen_vector(markers);
}

Ensure(can_create_markers_for_single_non_double) {
    CgreenVector *markers = create_vector_of_double_markers_for("a");
    assert_that(cgreen_vector_size(markers), is_equal_to(1));
    assert_that(!*(bool*)cgreen_vector_get(markers, 0));
    destroy_cgreen_vector(markers);
}

Ensure(can_create_markers_for_single_double) {
    CgreenVector *markers = create_vector_of_double_markers_for("box_double(a)");
    assert_that(cgreen_vector_size(markers), is_equal_to(1));
    assert_that(*(bool*)cgreen_vector_get(markers, 0));
    destroy_cgreen_vector(markers);
}

Ensure(can_create_markers_for_mixed_parameters) {
    CgreenVector *markers = create_vector_of_double_markers_for("a, box_double(b), c,d,box_double(e)");
    assert_that(cgreen_vector_size(markers), is_equal_to(5));
    assert_that(!*(bool*)cgreen_vector_get(markers, 0));
    assert_that(*(bool*)cgreen_vector_get(markers, 1));
    assert_that(!*(bool*)cgreen_vector_get(markers, 2));
    assert_that(!*(bool*)cgreen_vector_get(markers, 3));
    assert_that(*(bool*)cgreen_vector_get(markers, 4));
    destroy_cgreen_vector(markers);
}

TestSuite *parameter_tests(void) {
    TestSuite *suite = create_test_suite();
    set_teardown(suite, destroy_names);
    add_test(suite, can_create_vector_of_no_parameters_and_destroy_it);
    add_test(suite, can_read_single_parameter);
    add_test(suite, can_read_two_parameters);
    add_test(suite, can_read_three_parameters);
    add_test(suite, can_read_two_parameters_without_spaces);
    add_test(suite, can_read_long_parameters);
    add_test(suite, can_read_long_parameters_with_funky_names);
    add_test(suite, can_read_two_parameters_with_varied_whitespace);
    add_test(suite, can_strip_box_double_to_leave_original_name);
    add_test(suite, can_strip_two_box_doubles_to_leave_original_names);
    add_test(suite, can_strip_d_macro_to_leave_original_name);
    add_test(suite, can_strip_two_d_macros_to_leave_original_names);
    add_test(suite, can_strip_d_macro_and_box_double_to_leave_original_names);
    add_test(suite, can_strip_box_double_and_d_macro_to_leave_original_names);
    add_test(suite, can_strip_multiple_mixed_parameters_to_leave_original_names);
    return suite;
}
