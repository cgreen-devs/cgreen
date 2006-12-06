#include "../cgreen.h"
#include "../vector.h"
#include "../parameters.h"
#include <stdlib.h>

static Vector *names = NULL;

static void destroy_names() {
    destroy_vector(names);
    names = NULL;
}

static void can_create_vector_of_no_parameters_and_destroy_it() {
    names = create_vector_of_names("");
    assert_equal(vector_size(names), 0);
}

static void can_read_single_parameter() {
    names = create_vector_of_names("a");
    assert_equal(vector_size(names), 1);
    assert_string_equal(vector_get(names, 0), "a");
}

static void can_read_two_parameters() {
    names = create_vector_of_names("a, b");
    assert_equal(vector_size(names), 2);
    assert_string_equal(vector_get(names, 0), "a");
    assert_string_equal(vector_get(names, 1), "b");
}

static void can_read_three_parameters() {
    names = create_vector_of_names("a, b, c");
    assert_equal(vector_size(names), 3);
    assert_string_equal(vector_get(names, 0), "a");
    assert_string_equal(vector_get(names, 1), "b");
    assert_string_equal(vector_get(names, 2), "c");
}

static void can_read_two_parameters_without_spaces() {
    names = create_vector_of_names("a,b");
    assert_string_equal(vector_get(names, 0), "a");
    assert_string_equal(vector_get(names, 1), "b");
}

static void can_read_long_parameters() {
    names = create_vector_of_names("abacus, banana");
    assert_string_equal(vector_get(names, 0), "abacus");
    assert_string_equal(vector_get(names, 1), "banana");
}

static void can_read_long_parameters_with_funky_names() {
    names = create_vector_of_names("a_b-c+d, +a-d_c/d");
    assert_string_equal(vector_get(names, 0), "a_b-c+d");
    assert_string_equal(vector_get(names, 1), "+a-d_c/d");
}

static void can_read_two_parameters_with_varied_whitespace() {
    names = create_vector_of_names("\ra\t,\nb\t\t\t");
    assert_string_equal(vector_get(names, 0), "a");
    assert_string_equal(vector_get(names, 1), "b");
}

TestSuite *parameter_tests() {
    TestSuite *suite = create_test_suite();
    teardown(suite, destroy_names);
    add_test(suite, can_create_vector_of_no_parameters_and_destroy_it);
    add_test(suite, can_read_single_parameter);
    add_test(suite, can_read_two_parameters);
    add_test(suite, can_read_three_parameters);
    add_test(suite, can_read_two_parameters_without_spaces);
    add_test(suite, can_read_long_parameters);
    add_test(suite, can_read_long_parameters_with_funky_names);
    add_test(suite, can_read_two_parameters_with_varied_whitespace);
    return suite;
}
