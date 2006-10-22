#include "../cgreen.h"
#include "../vector.h"
#include <stdlib.h>

static Vector *vector;
static char a = 'a', b = 'b', c = 'c';

static void set_up_vector() {
    vector = create_vector(NULL);
}

static void tear_down_vector() {
    destroy_vector(vector);
}

static void new_vector_is_empty() {
    assert_equal(vector_size(vector), 0);
}

static void single_item_gives_count_of_one() {
    vector_add(vector, &a);
    assert_equal(vector_size(vector), 1);
}

static void single_item_is_readable() {
    vector_add(vector, &a);
    assert_equal(*(char *)vector_get(vector, 0), 'a');
}

static void double_item_gives_count_of_two() {
    vector_add(vector, &a);
    vector_add(vector, &b);
    assert_equal(vector_size(vector), 2);
}

static void two_items_are_readable() {
    vector_add(vector, &a);
    vector_add(vector, &b);
    assert_equal(*(char *)vector_get(vector, 0), 'a');
    assert_equal(*(char *)vector_get(vector, 1), 'b');
}

static void can_extract_only_item() {
    vector_add(vector, &a);
    assert_equal(*(char *)vector_remove(vector, 0), 'a');
    assert_equal(vector_size(vector), 0);
}

static void can_extract_head_item() {
    vector_add(vector, &a);
    vector_add(vector, &b);
    vector_add(vector, &c);
    assert_equal(*(char *)vector_remove(vector, 0), 'a');
    assert_equal(*(char *)vector_get(vector, 0), 'b');
    assert_equal(*(char *)vector_get(vector, 1), 'c');
}

static void can_extract_tail_item() {
    vector_add(vector, &a);
    vector_add(vector, &b);
    vector_add(vector, &c);
    assert_equal(*(char *)vector_remove(vector, 2), 'c');
    assert_equal(*(char *)vector_get(vector, 0), 'a');
    assert_equal(*(char *)vector_get(vector, 1), 'b');
}

static void can_extract_middle_item() {
    vector_add(vector, &a);
    vector_add(vector, &b);
    vector_add(vector, &c);
    assert_equal(*(char *)vector_remove(vector, 1), 'b');
    assert_equal(*(char *)vector_get(vector, 0), 'a');
    assert_equal(*(char *)vector_get(vector, 1), 'c');
}

static int times_called = 0;
static void sample_destructor(void *item) {
    times_called++;
}

static void destructor_is_called_on_single_item() {
    Vector *vector = create_vector(&sample_destructor);
    vector_add(vector, &a);
    destroy_vector(vector);
    assert_equal(times_called, 1);
}

static void destructor_is_not_called_on_empty_vector() {
    Vector *vector = create_vector(&sample_destructor);
    destroy_vector(vector);
    assert_equal(times_called, 0);
}

static void destructor_is_called_three_times_on_three_item_vector() {
    Vector *vector = create_vector(&sample_destructor);
    vector_add(vector, &a);
    vector_add(vector, &b);
    vector_add(vector, &c);
    destroy_vector(vector);
    assert_equal(times_called, 3);
}

static void vector_size_of_null_pointer_is_zero() {
    assert_equal(vector_size(NULL), 0);
}

TestSuite *vector_tests() {
    TestSuite *suite = create_test_suite();
    setup(suite, set_up_vector);
    teardown(suite, tear_down_vector);
    add_test(suite, new_vector_is_empty);
    add_test(suite, single_item_gives_count_of_one);
    add_test(suite, single_item_is_readable);
    add_test(suite, double_item_gives_count_of_two);
    add_test(suite, two_items_are_readable);
    add_test(suite, can_extract_only_item);
    add_test(suite, can_extract_head_item);
    add_test(suite, can_extract_tail_item);
    add_test(suite, can_extract_middle_item);
    add_test(suite, destructor_is_called_on_single_item);
    add_test(suite, destructor_is_not_called_on_empty_vector);
    add_test(suite, destructor_is_called_three_times_on_three_item_vector);
    add_test(suite, vector_size_of_null_pointer_is_zero);
    return suite;
}
