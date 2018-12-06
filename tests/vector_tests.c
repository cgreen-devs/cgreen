#include <cgreen/cgreen.h>
#include <cgreen/vector.h>
#include <stdlib.h>

#include "../src/utils.h"

#ifdef __cplusplus
using namespace cgreen;
#endif

static CgreenVector *vector;
static char a = 'a', b = 'b', c = 'c';
static int times_called = 0;

static void set_up_vector(void) {
    vector = create_cgreen_vector(NULL);
    times_called = 0;
}

static void tear_down_vector(void) {
    destroy_cgreen_vector(vector);
}

Describe(Vector);
BeforeEach(Vector) {
    set_up_vector();
}
AfterEach(Vector) {
    tear_down_vector();
}

Ensure(Vector, new_vector_is_empty) {
    assert_that(cgreen_vector_size(vector), is_equal_to(0));
}

Ensure(Vector, single_item_gives_count_of_one) {
    cgreen_vector_add(vector, &a);
    assert_that(cgreen_vector_size(vector), is_equal_to(1));
}

Ensure(Vector, single_item_is_readable) {
    cgreen_vector_add(vector, &a);
    assert_that(*(char *)cgreen_vector_get(vector, 0), is_equal_to('a'));
}

Ensure(Vector, double_item_gives_count_of_two) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    assert_that(cgreen_vector_size(vector), is_equal_to(2));
}

Ensure(Vector, two_items_are_readable) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    assert_that(*(char *)cgreen_vector_get(vector, 0), is_equal_to('a'));
    assert_that(*(char *)cgreen_vector_get(vector, 1), is_equal_to('b'));
}

Ensure(Vector, can_extract_only_item) {
    cgreen_vector_add(vector, &a);
    assert_that(*(char *)cgreen_vector_remove(vector, 0), is_equal_to('a'));
    assert_that(cgreen_vector_size(vector), is_equal_to(0));
}

Ensure(Vector, can_extract_head_item) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    assert_that(*(char *)cgreen_vector_remove(vector, 0), is_equal_to('a'));
    assert_that(*(char *)cgreen_vector_get(vector, 0), is_equal_to('b'));
    assert_that(*(char *)cgreen_vector_get(vector, 1), is_equal_to('c'));
}

Ensure(Vector, can_extract_tail_item) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    assert_that(*(char *)cgreen_vector_remove(vector, 2), is_equal_to('c'));
    assert_that(*(char *)cgreen_vector_get(vector, 0), is_equal_to('a'));
    assert_that(*(char *)cgreen_vector_get(vector, 1), is_equal_to('b'));
}

Ensure(Vector, can_extract_middle_item) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    assert_that(*(char *)cgreen_vector_remove(vector, 1), is_equal_to('b'));
    assert_that(*(char *)cgreen_vector_get(vector, 0), is_equal_to('a'));
    assert_that(*(char *)cgreen_vector_get(vector, 1), is_equal_to('c'));
}

static void sample_destructor(void *item) {
    (void)item;
    times_called++;
}

Ensure(Vector, destructor_is_called_on_single_item) {
    CgreenVector *vector = create_cgreen_vector(&sample_destructor);
    cgreen_vector_add(vector, &a);
    destroy_cgreen_vector(vector);
    assert_that(times_called, is_equal_to(1));
}

Ensure(Vector, destructor_is_not_called_on_empty_vector) {
    CgreenVector *vector = create_cgreen_vector(&sample_destructor);
    destroy_cgreen_vector(vector);
    assert_that(times_called, is_equal_to(0));
}

Ensure(Vector, destructor_is_called_three_times_on_three_item_vector) {
    CgreenVector *vector = create_cgreen_vector(&sample_destructor);
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    destroy_cgreen_vector(vector);
    assert_that(times_called, is_equal_to(3));
}

Ensure(Vector, vector_size_of_null_pointer_is_zero) {
    assert_that(cgreen_vector_size(NULL), is_equal_to(0));
}

Ensure(Vector, returns_null_for_get_on_illegal_index) {
    CgreenVector *vector = create_cgreen_vector(NULL);
    char panic_message[1000];

    panic_set_output_buffer(panic_message);

    assert_that(cgreen_vector_get(vector, -1), is_equal_to(NULL));
    assert_that(panic_message, contains_string("illegal position (-1) in vector operation"));
    destroy_cgreen_vector(vector);
}

Ensure(Vector, returns_null_for_remove_from_illegal_index) {
    CgreenVector *vector = create_cgreen_vector(NULL);
    char panic_message[1000];

    panic_set_output_buffer(panic_message);

    assert_that(cgreen_vector_remove(vector, 1), is_equal_to(NULL));
    assert_that(panic_message, contains_string("illegal position (1) in vector operation"));
    destroy_cgreen_vector(vector);
}

TestSuite *vector_tests(void) {
    TestSuite *suite = create_test_suite();
    set_setup(suite, set_up_vector);
    set_teardown(suite, tear_down_vector);
    add_test_with_context(suite, Vector, new_vector_is_empty);
    add_test_with_context(suite, Vector, single_item_gives_count_of_one);
    add_test_with_context(suite, Vector, single_item_is_readable);
    add_test_with_context(suite, Vector, double_item_gives_count_of_two);
    add_test_with_context(suite, Vector, two_items_are_readable);
    add_test_with_context(suite, Vector, can_extract_only_item);
    add_test_with_context(suite, Vector, can_extract_head_item);
    add_test_with_context(suite, Vector, can_extract_tail_item);
    add_test_with_context(suite, Vector, can_extract_middle_item);
    add_test_with_context(suite, Vector, destructor_is_called_on_single_item);
    add_test_with_context(suite, Vector, destructor_is_not_called_on_empty_vector);
    add_test_with_context(suite, Vector, destructor_is_called_three_times_on_three_item_vector);
    add_test_with_context(suite, Vector, vector_size_of_null_pointer_is_zero);
    add_test_with_context(suite, Vector, returns_null_for_get_on_illegal_index);
    add_test_with_context(suite, Vector, returns_null_for_remove_from_illegal_index);
    return suite;
}
