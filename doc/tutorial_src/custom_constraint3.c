#include <cgreen/cgreen.h>
#include "cgreen_value_internal.h"
#include "utils.h"

Describe(TestConstraint);
BeforeEach(TestConstraint) {}
AfterEach(TestConstraint) {}

typedef struct Box {
    int id;
    int size;
} Box;

typedef struct Piece {
    int id;
    int size;
} Piece;

bool compare_piece_and_box_size(Constraint *constraint, CgreenValue actual) {
    return ((Piece *)actual.value.pointer_value)->size
        < ((Box*)constraint->expected_value.value.pointer_value)->size ;
}

static void test_fit_piece(Constraint *constraint, const char *function_name, CgreenValue actual,
                           const char *test_file, int test_line, TestReporter *reporter) {
    (*reporter->assert_true)(
            reporter,
            test_file,
            test_line,
            (*constraint->compare)(constraint, actual),
            "Piece [%f], does not fit in [%f] in function [%s] parameter [%s]",
            ((Piece *)constraint->expected_value.value.pointer_value)->id,
            ((Box *)actual.value.pointer_value)->id,
            function_name,
            constraint->parameter_name);
}

Constraint *create_piece_fit_in_box_constraint(intptr_t expected_value, const char *expected_value_name) {
    Constraint *constraint = create_constraint();

    constraint->expected_value = make_cgreen_pointer_value((void*)expected_value);
    constraint->expected_value_name = string_dup(expected_value_name);
    constraint->type = CONTENT_COMPARER;

    constraint->compare = &compare_piece_and_box_size;
    constraint->execute = &test_fit_piece;
    constraint->name = "fit in box";
    constraint->size_of_expected_value = sizeof(intptr_t);

    return constraint;
}
#define can_fit_in_box(box) create_piece_fit_in_box_constraint((intptr_t)box, #box)

Ensure(TestConstraint, more_complex_custom_constraint_function) {
    Box box1 = {.id = 1, .size = 5};
    Piece piece99 = {.id = 99, .size = 6};
    assert_that(&piece99, can_fit_in_box(&box1));
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, TestConstraint, more_complex_custom_constraint_function);
    run_test_suite(suite, create_text_reporter());
}
