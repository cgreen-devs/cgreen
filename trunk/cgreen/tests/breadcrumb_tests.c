#include "../cgreen.h"
#include "../breadcrumb.h"
#include <stdlib.h>

void can_destroy_empty_breadcrumb() {
    destroy_breadcrumb(create_breadcrumb());
}

void last_name_pushed_is_current() {
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    assert_string_equal(get_current_from_breadcrumb(breadcrumb), "Hello");
    destroy_breadcrumb(breadcrumb);
}

void can_push_more_than_one_item() {
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    assert_string_equal(get_current_from_breadcrumb(breadcrumb), "Goodbye");
}

void popping_item_takes_us_back_to_the_previous_item() {
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    pop_breadcrumb(breadcrumb);
    assert_string_equal(get_current_from_breadcrumb(breadcrumb), "Hello");
}

void empty_breadcrumb_has_null_as_current() {
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    assert_equal(get_current_from_breadcrumb(breadcrumb), NULL);
}

void popping_last_name_leaves_breadcrumb_empty() {
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    pop_breadcrumb(breadcrumb);
    assert_equal(get_current_from_breadcrumb(breadcrumb), NULL);
}

void mock_walker(const char *name, void *memo) {
    mock(name, memo);
}

void empty_breadcrumb_does_not_trigger_walker() {
    expect_never(mock_walker);
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    walk_breadcrumb(breadcrumb, &mock_walker, NULL);
}

void single_item_breadcrumb_does_calls_walker_only_once() {
    expect(mock_walker, want_string(name, "Hello"));
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    walk_breadcrumb(breadcrumb, &mock_walker, NULL);
}

void double_item_breadcrumb_does_calls_walker_only_once() {
    expect(mock_walker, want_string(name, "Hello"));
    expect(mock_walker, want_string(name, "Goodbye"));
    CgreenBreadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    walk_breadcrumb(breadcrumb, &mock_walker, NULL);
}

TestSuite *breadcrumb_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, can_destroy_empty_breadcrumb);
    add_test(suite, last_name_pushed_is_current);
    add_test(suite, can_push_more_than_one_item);
    add_test(suite, popping_item_takes_us_back_to_the_previous_item);
    add_test(suite, empty_breadcrumb_has_null_as_current);
    add_test(suite, popping_last_name_leaves_breadcrumb_empty);
    add_test(suite, empty_breadcrumb_does_not_trigger_walker);
    add_test(suite, single_item_breadcrumb_does_calls_walker_only_once);
    add_test(suite, double_item_breadcrumb_does_calls_walker_only_once);
    return suite;
}
