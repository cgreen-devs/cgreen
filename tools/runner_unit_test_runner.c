#include "runner_unit_tests.c"
TestSuite *runner_unit_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Runner, can_get_context_name_of_name);
    add_test_with_context(suite, Runner, can_get_test_name_of_symbolic_name);
    add_test_with_context(suite, Runner, can_ensure_test_exists_from_context_and_name);
    add_test_with_context(suite, Runner, can_match_test_name);
    add_test_with_context(suite, Runner, can_add_test_to_the_suite_for_its_context);
    add_test_with_context(suite, Runner, can_sort_an_empty_list_of_tests);
    add_test_with_context(suite, Runner, can_sort_a_list_of_a_single_tests);
    add_test_with_context(suite, Runner, can_sort_a_list_of_two_unordered_tests);
    add_test_with_context(suite, Runner, can_sort_an_ordered_list_of_two_tests);
    add_test_with_context(suite, Runner, can_sort_an_unordered_list_of_tests);
    return suite;
}
int main(int argc, char **argv) {
    return run_test_suite(runner_unit_tests(), create_text_reporter());
}
