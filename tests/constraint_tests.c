#include <cgreen/boxed_double.h>
#include <cgreen/cgreen.h>
#include <cgreen/constraint.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

#include "../src/cgreen_value_internal.h"
#include "../src/constraint_internal.h"

static bool compare_integer_constraint(Constraint *c, int x) {
    CgreenValue v = make_cgreen_integer_value(x);
    bool r = (*c->compare)(c, v);
    destroy_cgreen_value(v);
    return r;
}

static bool compare_pointer_constraint(Constraint *c, void *x) {
    CgreenValue v = make_cgreen_pointer_value(x);
    bool r = (*c->compare)(c, v);
    destroy_cgreen_value(v);
    return r;
}

static bool compare_string_constraint(Constraint *c, const char *x) {
    CgreenValue v = make_cgreen_string_value(x);
    bool r = (*c->compare)(c, v);
    destroy_cgreen_value(v);
    return r;
}

static bool compare_double_constraint(Constraint *c, double x) {
    CgreenValue v = make_cgreen_double_value(x);
    bool r = (*c->compare)(c, v);
    destroy_cgreen_value(v);
    return r;
}

Describe(Constraint);
BeforeEach(Constraint) {}
AfterEach(Constraint) {}

Ensure(Constraint, default_destroy_clears_state) {
    Constraint *constraint = create_constraint();
    destroy_constraint(constraint);

/* these tests correctly trip valgrind's use-after-free check, so
 * uncomment and check manually

    assert_that(constraint->name, NULL);
    assert_that(constraint->parameter, NULL);
    assert_that(constraint->storedValue, NULL);
    assert_that(constraint->test, NULL);
    assert_that(constraint->compare, NULL);
    assert_that(constraint->destroy, NULL);
 */
}

Ensure(Constraint, parameter_name_matches_correctly) {
    Constraint *constraint =
            create_constraint();
    constraint->type = VALUE_COMPARER;
    constraint->parameter_name = "label";

    assert_that(constraint_is_not_for_parameter(constraint, "wrong_label"), is_true);
    assert_that(constraint_is_not_for_parameter(constraint, "label"), is_false);

    destroy_constraint(constraint);
}

Ensure(Constraint, compare_contents_is_correct_on_larger_than_intptr_array) {
    int content[] = { 0, 1, 2, 3, 4, 5, 6, 7 ,8 ,9, 10, 11, 12, 13, 14, 15 };
    int also_content[] = { 0, 1, 2, 3, 4, 5, 6, 7 ,8 ,9, 10, 11, 12, 13, 14, 15 };
    Constraint *is_equal_to_contents_constraint =
            create_equal_to_contents_constraint(content, sizeof(content), "content");

    int not_content[] = { 0, 1, 2, 3, 4, 5, 6, 7, 108, 109, 110, 111, 112, 113, 114, 115 };
    assert_that(compare_pointer_constraint(is_equal_to_contents_constraint, also_content), is_true);
    assert_that(compare_pointer_constraint(is_equal_to_contents_constraint, not_content), is_false);

    destroy_constraint(is_equal_to_contents_constraint);
}

Ensure(Constraint, compare_is_correct_when_using_integers) {
    Constraint *is_equal_to_37 = create_equal_to_value_constraint(37, "37");

    assert_that(compare_integer_constraint(is_equal_to_37, 37), is_true);
    assert_that(compare_integer_constraint(is_equal_to_37, 36), is_false);

    destroy_constraint(is_equal_to_37);
}

Ensure(Constraint, compare_to_is_null_correctly) {
    assert_that(0, is_null);
    assert_that(14, is_not_null);
}

Ensure(Constraint, string_constraint_destroy_clears_state) {
    Constraint *string_constraint =
            create_equal_to_string_constraint("Hello", "user_greeting");
    destroy_constraint(string_constraint);

/* these checks correctly trip valgrind's use-after-free check, so
 * uncomment and check manually

    assert_that(string_constraint->name, is_null);
    assert_that(string_constraint->parameter, is_null);
    assert_that(string_constraint->storedValue, is_null);
    assert_that(string_constraint->test, is_null);
    assert_that(string_constraint->compare, is_null);
    assert_that(string_constraint->destroy, is_null);
 */
}

Ensure(Constraint, matching_strings_as_equal) {
    Constraint *equals_string_hello_constraint =
            create_equal_to_string_constraint("Hello", "user_greeting");

    assert_that(compare_string_constraint(equals_string_hello_constraint, "Hello"), is_true);
    assert_that(compare_string_constraint(equals_string_hello_constraint, "Goodbye"), is_false);

    destroy_constraint(equals_string_hello_constraint);
}

Ensure(Constraint, matching_null_string_against_non_null_string) {
    Constraint *equals_string_hello_constraint =
            create_equal_to_string_constraint("Hello", "user_greeting");

    assert_that(compare_string_constraint(equals_string_hello_constraint, NULL), is_false);

    destroy_constraint(equals_string_hello_constraint);
}

Ensure(Constraint, matching_against_null_string) {
    Constraint *equals_null_string_constraint =
            create_equal_to_string_constraint((const char *)NULL, "user_greeting");

    assert_that(compare_string_constraint(equals_null_string_constraint, NULL), is_true);
    assert_that(compare_string_constraint(equals_null_string_constraint, "Hello"), is_false);

    destroy_constraint(equals_null_string_constraint);
}

Ensure(Constraint, matching_doubles_as_equal_with_default_significance) {
    Constraint *equal_to_double_37 = create_equal_to_double_constraint(37.0, "height");

    assert_that(compare_double_constraint(equal_to_double_37, 37.0), is_true);
    assert_that(compare_double_constraint(equal_to_double_37, 36.0), is_false);

    destroy_constraint(equal_to_double_37);
}

Ensure(Constraint, matching_doubles_respects_significant_figure_setting) {
    Constraint *want_337 = create_equal_to_double_constraint(337.0, "height");

    significant_figures_for_assert_double_are(2);
    assert_that(compare_double_constraint(want_337, 339.0), is_true);

    significant_figures_for_assert_double_are(3);
    assert_that(compare_double_constraint(want_337, 339.0), is_false);

    destroy_constraint(want_337);
}

// TODO: convert to create_constraint_for_comparator("non-empty string", &is_non_empty_string);
//typedef struct {
//	char* value;
//	unsigned int length;
//} String;
//
//static int is_non_empty_string(const void* other) {
//	String* our = (String*)other;
//	return our->length != 0;
//}
//
//Ensure(Constraint, unequal_structs_with_same_value_for_specific_field_compare_true) {
//    String name;
//    name.value = "bob";
//    name.length = 3;
//
//    Constraint *string_constraint = with(name, is_non_empty_string);
//
//    assert_that(compare_constraint(string_constraint, is_non_empty_string), is_equal_to(1));
//
//    destroy_constraint(string_constraint);
//
//    name.value = "tim";
//    name.length = 3;
//
//    string_constraint = with(name, is_non_empty_string);
//    assert_that(compare_constraint(string_constraint, is_non_empty_string), is_equal_to(1));
//
//    destroy_constraint(string_constraint);
//}


/* these misuse scenarios should be prevented by checks in higher-level constructs */
#ifdef CANNOT_CREATE_NULL_CONSTRAINTS
Ensure(Constraint, cannot_create_contents_constraint_with_null_content) {
    const size_t NON_ZERO = !0;
    Constraint *is_equal_to_contents_constraint = create_equal_to_contents_constraint(NULL, NON_ZERO, "NULL");

    assert_that(is_equal_to_contents_constraint, is_null);
}

Ensure(Constraint, cannot_create_contents_constraint_with_zero_size) {
    int content[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    Constraint *is_equal_to_contents_constraint = create_equal_to_contents_constraint(content, 0, "content");

    assert_that(is_equal_to_contents_constraint, is_null);
}
#endif

Ensure(Constraint, compare_equal_to_contents_is_false_on_null) {
    int content[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    Constraint *is_equal_to_contents_constraint =
        create_equal_to_contents_constraint(content, sizeof(content), "content");

    assert_that(compare_pointer_constraint(is_equal_to_contents_constraint, NULL), is_false);

    destroy_constraint(is_equal_to_contents_constraint);
}

Ensure(Constraint, compare_not_equal_to_contents_is_false_on_null) {
    int content[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    Constraint *is_not_equal_to_contents =
        create_not_equal_to_contents_constraint(content, sizeof(content), "content");

    assert_that(compare_pointer_constraint(is_not_equal_to_contents, NULL), is_false);

    destroy_constraint(is_not_equal_to_contents);
}

Ensure(Constraint, can_compare_to_hex) {
    char chars[3];
    memset(chars, 0xaa, sizeof(chars));
    assert_that((unsigned char)chars[0], is_equal_to_hex(0xaa));
}

TestSuite *constraint_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Constraint, default_destroy_clears_state);
    add_test_with_context(suite, Constraint, parameter_name_matches_correctly);
    add_test_with_context(suite, Constraint, compare_is_correct_when_using_integers);
    add_test_with_context(suite, Constraint, string_constraint_destroy_clears_state);
    add_test_with_context(suite, Constraint, matching_strings_as_equal);
    add_test_with_context(suite, Constraint, matching_null_string_against_non_null_string);
    add_test_with_context(suite, Constraint, matching_against_null_string);
    add_test_with_context(suite, Constraint, matching_doubles_as_equal_with_default_significance);
    add_test_with_context(suite, Constraint, matching_doubles_respects_significant_figure_setting);
    add_test_with_context(suite, Constraint, compare_contents_is_correct_on_larger_than_intptr_array);
    add_test_with_context(suite, Constraint, compare_equal_to_contents_is_false_on_null);
    add_test_with_context(suite, Constraint, compare_not_equal_to_contents_is_false_on_null);
    add_test_with_context(suite, Constraint, can_compare_to_hex);
//    add_test_with_context(suite, Constraint, unequal_structs_with_same_value_for_specific_field_compare_true);

    return suite;
}
