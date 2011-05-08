#include <cgreen/cgreen.h>
#include <cgreen/vector.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static CgreenVector *vector;
static char a = 'a', b = 'b', c = 'c';

static void set_up_vector() {
    vector = create_cgreen_vector(NULL);
}

static void tear_down_vector() {
    destroy_cgreen_vector(vector);
}

Ensure(new_vector_is_empty) {
    assert_equal(cgreen_vector_size(vector), 0);
}

Ensure(single_item_gives_count_of_one) {
    cgreen_vector_add(vector, &a);
    assert_equal(cgreen_vector_size(vector), 1);
}

Ensure(single_item_is_readable) {
    cgreen_vector_add(vector, &a);
    assert_equal(*(char *)cgreen_vector_get(vector, 0), 'a');
}

Ensure(double_item_gives_count_of_two) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    assert_equal(cgreen_vector_size(vector), 2);
}

Ensure(two_items_are_readable) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    assert_equal(*(char *)cgreen_vector_get(vector, 0), 'a');
    assert_equal(*(char *)cgreen_vector_get(vector, 1), 'b');
}

Ensure(can_extract_only_item) {
    cgreen_vector_add(vector, &a);
    assert_equal(*(char *)cgreen_vector_remove(vector, 0), 'a');
    assert_equal(cgreen_vector_size(vector), 0);
}

Ensure(can_extract_head_item) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    assert_equal(*(char *)cgreen_vector_remove(vector, 0), 'a');
    assert_equal(*(char *)cgreen_vector_get(vector, 0), 'b');
    assert_equal(*(char *)cgreen_vector_get(vector, 1), 'c');
}

Ensure(can_extract_tail_item) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    assert_equal(*(char *)cgreen_vector_remove(vector, 2), 'c');
    assert_equal(*(char *)cgreen_vector_get(vector, 0), 'a');
    assert_equal(*(char *)cgreen_vector_get(vector, 1), 'b');
}

Ensure(can_extract_middle_item) {
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    assert_equal(*(char *)cgreen_vector_remove(vector, 1), 'b');
    assert_equal(*(char *)cgreen_vector_get(vector, 0), 'a');
    assert_equal(*(char *)cgreen_vector_get(vector, 1), 'c');
}

static int times_called = 0;
static void sample_destructor(void *item) {
    (void)item;
    times_called++;
}

Ensure(destructor_is_called_on_single_item) {
    CgreenVector *vector = create_cgreen_vector(&sample_destructor);
    cgreen_vector_add(vector, &a);
    destroy_cgreen_vector(vector);
    assert_equal(times_called, 1);
}

Ensure(destructor_is_not_called_on_empty_vector) {
    CgreenVector *vector = create_cgreen_vector(&sample_destructor);
    destroy_cgreen_vector(vector);
    assert_equal(times_called, 0);
}

Ensure(destructor_is_called_three_times_on_three_item_vector) {
    CgreenVector *vector = create_cgreen_vector(&sample_destructor);
    cgreen_vector_add(vector, &a);
    cgreen_vector_add(vector, &b);
    cgreen_vector_add(vector, &c);
    destroy_cgreen_vector(vector);
    assert_equal(times_called, 3);
}

Ensure(vector_size_of_null_pointer_is_zero) {
    assert_equal(cgreen_vector_size(NULL), 0);
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
