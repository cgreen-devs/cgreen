#include "../cgreen.h"
#include "../breadcrumb.h"
#include <stdlib.h>

void can_destroy_empty_breadcrumb() {
    destroy_breadcrumb(create_breadcrumb());
}

void last_name_pushed_is_current() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    assert_string_equal(get_current_from_breadcrumb(breadcrumb), "Hello");
    destroy_breadcrumb(breadcrumb);
}

void can_push_more_than_one_item() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    assert_string_equal(get_current_from_breadcrumb(breadcrumb), "Goodbye");
}

void popping_item_takes_us_back_to_the_previous_item() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    pop_breadcrumb(breadcrumb);
    assert_string_equal(get_current_from_breadcrumb(breadcrumb), "Hello");
}

void empty_breadcrumb_has_null_as_current() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    assert_equal(get_current_from_breadcrumb(breadcrumb), NULL);
}

void popping_last_name_leaves_breadcrumb_empty() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    pop_breadcrumb(breadcrumb);
    assert_equal(get_current_from_breadcrumb(breadcrumb), NULL);
}

void walker_for_empty_breadcrumb(const char *name, void *memo) {
    //called_never();
}

void empty_breadcrumb_does_not_trigger_walker() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    walk_breadcrumb(breadcrumb, &walker_for_empty_breadcrumb, NULL);
}

void walker_for_single_item_breadcrumb(const char *name, void *memo) {
    //called_once();
    assert_string_equal(name, "Hello");
}

void single_item_breadcrumb_does_calls_walker_only_once() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    walk_breadcrumb(breadcrumb, &walker_for_single_item_breadcrumb, NULL);
}

void walker_for_double_item_breadcrumb(const char *name, void *memo) {
    //expected_call_count(2);
    //assert_string_equal(name, string_sequence("Hello", "Goodbye"), NULL);
}

void double_item_breadcrumb_does_calls_walker_only_once() {
    Breadcrumb *breadcrumb = create_breadcrumb();
    push_breadcrumb(breadcrumb, "Hello");
    push_breadcrumb(breadcrumb, "Goodbye");
    walk_breadcrumb(breadcrumb, &walker_for_double_item_breadcrumb, NULL);
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
