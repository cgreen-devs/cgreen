#include "../cgreen.h"
#include "../vector.h"
#include "../parameters.h"

static void can_create_vector_of_no_parameters_and_destroy_it() {
    Vector *names = create_vector_of_names("");
    assert_equal(vector_size(names), 0);
    destroy_vector(names);
}

TestSuite *parameter_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_create_vector_of_no_parameters_and_destroy_it);
    return suite;
}
