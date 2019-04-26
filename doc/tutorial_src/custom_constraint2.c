#include <cgreen/cgreen.h>
#include <cgreen/message_formatting.h>
#include "cgreen_value_internal.h"
#include "utils.h"

Describe(TestConstraint);
BeforeEach(TestConstraint) {}
AfterEach(TestConstraint) {}

bool compare_want_smaller_value(Constraint *constraint, CgreenValue actual) {
    return actual.value.integer_value < constraint->expected_value.value.integer_value ;
}

Constraint *create_smaller_than_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint();

    constraint->expected_value = make_cgreen_integer_value(expected_value);
    constraint->expected_value_name = string_dup(expected_value_name);
    constraint->type = VALUE_COMPARER;

    constraint->compare = &compare_want_smaller_value;
    constraint->execute = &test_want;
    constraint->name = "be smaller than";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}
#define is_smaller_than(value) create_smaller_than_constraint(value, #value)


Ensure(TestConstraint, custom_constraint_using_a_function_with_arguments_function) {
    assert_that(9, is_smaller_than(10));
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, TestConstraint, custom_constraint_using_a_function_with_arguments_function);
    run_test_suite(suite, create_text_reporter());
}
