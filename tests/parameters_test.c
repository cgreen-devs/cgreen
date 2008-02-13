#include "../cgreen.h"
#include "../vector.h"
#include "../parameters.h"
#include <stdlib.h>

static CgreenVector *names = NULL;

Ensure destroy_names() {
    destroy_cgreen_vector(names);
    names = NULL;
}

Ensure can_create_vector_of_no_parameters_and_destroy_it() {
    names = create_vector_of_names("");
    assert_equal(cgreen_vector_size(names), 0);
}

Ensure can_read_single_parameter() {
    names = create_vector_of_names("a");
    assert_equal(cgreen_vector_size(names), 1);
    assert_string_equal(cgreen_vector_get(names, 0), "a");
}

Ensure can_read_two_parameters() {
    names = create_vector_of_names("a, b");
    assert_equal(cgreen_vector_size(names), 2);
    assert_string_equal(cgreen_vector_get(names, 0), "a");
    assert_string_equal(cgreen_vector_get(names, 1), "b");
}

Ensure can_read_three_parameters() {
    names = create_vector_of_names("a, b, c");
    assert_equal(cgreen_vector_size(names), 3);
    assert_string_equal(cgreen_vector_get(names, 0), "a");
    assert_string_equal(cgreen_vector_get(names, 1), "b");
    assert_string_equal(cgreen_vector_get(names, 2), "c");
}

Ensure can_read_two_parameters_without_spaces() {
    names = create_vector_of_names("a,b");
    assert_string_equal(cgreen_vector_get(names, 0), "a");
    assert_string_equal(cgreen_vector_get(names, 1), "b");
}

Ensure can_read_long_parameters() {
    names = create_vector_of_names("abacus, banana");
    assert_string_equal(cgreen_vector_get(names, 0), "abacus");
    assert_string_equal(cgreen_vector_get(names, 1), "banana");
}

Ensure can_read_long_parameters_with_funky_names() {
    names = create_vector_of_names("a_b-c+d, +a-d_c/d");
    assert_string_equal(cgreen_vector_get(names, 0), "a_b-c+d");
    assert_string_equal(cgreen_vector_get(names, 1), "+a-d_c/d");
}

Ensure can_read_two_parameters_with_varied_whitespace() {
    names = create_vector_of_names("\ra\t,\nb\t\t\t");
    assert_string_equal(cgreen_vector_get(names, 0), "a");
    assert_string_equal(cgreen_vector_get(names, 1), "b");
}

TestSuite *parameter_tests() {
    TestSuite *suite = create_test_suite();
    teardown(suite, destroy_names);
    //add_test(suite, can_create_vector_of_no_parameters_and_destroy_it);
    //add_test(suite, can_read_single_parameter);
    //add_test(suite, can_read_two_parameters);
    //add_test(suite, can_read_three_parameters);
    //add_test(suite, can_read_two_parameters_without_spaces);
    //add_test(suite, can_read_long_parameters);
    //add_test(suite, can_read_long_parameters_with_funky_names);
    //add_test(suite, can_read_two_parameters_with_varied_whitespace);
    add_tests(suite, can_create_vector_of_no_parameters_and_destroy_it,
                     can_read_single_parameter,
                     can_read_two_parameters,
                     can_read_three_parameters,
                     can_read_two_parameters_without_spaces,
                     can_read_long_parameters,
                     can_read_long_parameters_with_funky_names,
                     can_read_two_parameters_with_varied_whitespace);
    return suite;
}
