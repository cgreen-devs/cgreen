#include <cgreen/cgreen.h>
#include <cgreen/vector.h>
#include <cgreen/parameters.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static CgreenVector *names = NULL;

void destroy_names() {
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

TestSuite *parameter_tests() {
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
    return suite;
}
