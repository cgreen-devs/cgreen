#include <cgreen/cgreen.h>
#include <cgreen/message_formatting.h>
#include "constraint_internal.h"

Describe(TestConstraint);
BeforeEach(TestConstraint) {}
AfterEach(TestConstraint) {}

bool compare_want_greater_than_5(Constraint *constraint, CgreenValue actual) {
    return actual.value.integer_value > 5;
}

Constraint static_is_bigger_than_5 = {
        /* .type */ VALUE_COMPARER,
        /* .name */ "be bigger than 5",
        /* .destroy */ destroy_static_constraint,
        /* .compare */ compare_want_greater_than_5,
        /* .test */ test_want,
        /* .format_failure_message_for */ failure_message_for,
        /* .actual_value_message */ "",
        /* .expected_value_message */ "",
        /* .expected_value */ {INTEGER, {5}},
        /* .stored_value_name */ "null",
        /* .parameter_name */ NULL,
        /* .size_of_stored_value */ 0
};

Ensure(TestConstraint, custom_constraint_using_static_function) {
    Constraint * is_bigger_than_5 = &static_is_bigger_than_5;
    assert_that(10, is_bigger_than_5);
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, TestConstraint, custom_constraint_using_static_function);
    run_test_suite(suite, create_text_reporter());
}
