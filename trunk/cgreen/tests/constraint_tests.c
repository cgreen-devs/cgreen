#include "../cgreen.h"
#include "../constraint.h"
#include <stdlib.h>

static void can_construct_and_destroy_is_constraint() {
    CgreenConstraint *constraint = is(label, 37);
    //(*constraint->destroy)(constraint);
}

TestSuite *constraint_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_construct_and_destroy_is_constraint);
    return suite;
}
