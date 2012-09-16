#include <cgreen/breadcrumb.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

static CgreenBreadcrumb* breadcrumb;

static void breadcrumb_tests_setup() {
    breadcrumb = create_breadcrumb();
}

static void breadcrumb_tests_teardown() {
    destroy_breadcrumb(breadcrumb);
}

Ensure(can_destroy_empty_breadcrumb) {
    destroy_breadcrumb(create_breadcrumb());
}

Ensure(last_name_pushed_is_current) {
    push_breadcrumb(breadcrumb, "Hello");
    assert_that(get_current_from_breadcrumb(breadcrumb), is_equal_to_string("Hello"));
}

Ensure(can_push_more_than_one_item) {
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    assert_that(get_current_from_breadcrumb(breadcrumb), is_equal_to_string("Goodbye"));
}

Ensure(popping_item_takes_us_back_to_the_previous_item) {
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    pop_breadcrumb(breadcrumb);
    assert_that(get_current_from_breadcrumb(breadcrumb), is_equal_to_string("Hello"));
}

Ensure(empty_breadcrumb_has_null_as_current) {
    assert_that(get_current_from_breadcrumb(breadcrumb), is_null);
}

Ensure(popping_last_name_leaves_breadcrumb_empty) {
    push_breadcrumb(breadcrumb, "Hello");
    pop_breadcrumb(breadcrumb);
    assert_that(get_current_from_breadcrumb(breadcrumb), is_null);
}

void mock_walker(const char *name, void *memo) {
    mock(name, memo);
}

Ensure(empty_breadcrumb_does_not_trigger_walker) {
    never_expect(mock_walker);
    walk_breadcrumb(breadcrumb, &mock_walker, NULL);
}

Ensure(single_item_breadcrumb_does_calls_walker_only_once) {
    expect(mock_walker, when(name, is_equal_to_string("Hello")));
    push_breadcrumb(breadcrumb, "Hello");
    walk_breadcrumb(breadcrumb, &mock_walker, NULL);
}

Ensure(double_item_breadcrumb_does_calls_walker_only_once) {
    expect(mock_walker, when(name, is_equal_to_string("Hello")));
    expect(mock_walker, when(name, is_equal_to_string("Goodbye")));
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    walk_breadcrumb(breadcrumb, &mock_walker, NULL);
}

TestSuite *breadcrumb_tests() {
    TestSuite *suite = create_test_suite();
    set_setup(suite, breadcrumb_tests_setup);

    add_test(suite, can_destroy_empty_breadcrumb);
    add_test(suite, last_name_pushed_is_current);
    add_test(suite, can_push_more_than_one_item);
    add_test(suite, popping_item_takes_us_back_to_the_previous_item);
    add_test(suite, empty_breadcrumb_has_null_as_current);
    add_test(suite, popping_last_name_leaves_breadcrumb_empty);
    add_test(suite, empty_breadcrumb_does_not_trigger_walker);
    add_test(suite, single_item_breadcrumb_does_calls_walker_only_once);
    add_test(suite, double_item_breadcrumb_does_calls_walker_only_once);

    set_teardown(suite, breadcrumb_tests_teardown);

    return suite;
}
