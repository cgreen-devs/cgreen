#include <cgreen/cgreen.h>

#include "utils.h"

#ifdef __cplusplus
using namespace cgreen;
#endif

#include "../runner.c"
#include "test_item.h"

Describe(Runner);

BeforeEach(Runner){}

AfterEach(Runner){}

#define CONTEXT_NAME "context"
#define TEST_NAME "test"

#define STRINGIFY_X(x) #x
#define STRINGIFY(x) STRINGIFY_X(x)

Ensure(Runner, can_get_context_name_of_name) {
    char *context_name = context_name_of("Context:Test");
    assert_that(context_name, is_equal_to_string("Context"));
    free(context_name);

    context_name = context_name_of("Test");
    assert_that(context_name, is_equal_to_string(CGREEN_DEFAULT_SUITE));
    free(context_name);
}


Ensure(Runner, can_get_test_name_of_symbolic_name) {
    char *test_name = test_name_of("Context:Test");
    assert_that(test_name, is_equal_to_string("Test"));
    free(test_name);

    test_name = test_name_of("Test");
    assert_that(test_name, is_equal_to_string("Test"));
    free(test_name);
}


static void add_test_items_to_vector(TestItem items[], CgreenVector *test_items, int count) {
    for (int i=0; i < count; i++)
        cgreen_vector_add(test_items, &items[i]);
}


Ensure(Runner, can_ensure_test_exists_from_context_and_name) {
    TestItem test_items[5] = {
        {(char *)"", (char *)"Context1", (char *)"Test1"},
        {(char *)"", (char *)"Context1", (char *)"Test2"},
        {(char *)"", (char *)"Context2", (char *)"Test1"},
        {(char *)"", (char *)"Context2", (char *)"Test2"}};
    CgreenVector *tests = create_cgreen_vector(NULL);
    add_test_items_to_vector(test_items, tests, 5);

    assert_that(matching_test_exists("Context1:Test1", tests));
}

Ensure(Runner, can_match_test_name) {
    TestItem test_item = {(char *)"", (char *)"Context1", (char *)"Test1"};

    assert_that(test_matches_pattern("Context1:Test1", &test_item));
    assert_that(test_matches_pattern("Context*:Test1", &test_item));
    assert_that(test_matches_pattern("*:Test1", &test_item));

    assert_that(test_matches_pattern("Context*:Test1", &test_item));
    assert_that(test_matches_pattern("*:Test1", &test_item));

    assert_that(test_matches_pattern("Context1:Test*", &test_item));
    assert_that(test_matches_pattern("Context*:Test*", &test_item));
    assert_that(test_matches_pattern("Context*:*", &test_item));
    assert_that(test_matches_pattern("*:Test*", &test_item));
    assert_that(test_matches_pattern("*:*", &test_item));
}

Ensure(Runner, can_add_test_to_the_suite_for_its_context) {
    ContextSuite *suite_list = NULL;
    CgreenTest *test = (CgreenTest *)&test;
    TestSuite *parent_suite = create_test_suite();
    TestSuite *first_suite, *second_suite;
    TestItem test_item1 = {"", "TheFirstContext", "TheName"};
    TestItem test_item2 = {"", "TheSecondContext", "TheName"};

    assert_that(suite_list, is_null);

    add_test_to_context(parent_suite, &suite_list, &test_item1, test);
    first_suite = find_suite_for_context(suite_list, "TheFirstContext");
    assert_that(first_suite, is_non_null);
    assert_that(first_suite->size, is_equal_to(1));

    second_suite = find_suite_for_context(suite_list, "TheSecondContext");
    assert_that(second_suite, is_null);

    add_test_to_context(parent_suite, &suite_list, &test_item2, test);
    assert_that(find_suite_for_context(suite_list, "TheFirstContext")->size, is_equal_to(1));
    assert_that(find_suite_for_context(suite_list, "TheSecondContext")->size, is_equal_to(1));

    destroy_test_suite(parent_suite);
    destroy_context_suites(suite_list);
}

Ensure(Runner, can_sort_an_empty_list_of_tests) {
    CgreenVector *test_items = create_cgreen_vector(NULL);

    test_items = sorted_test_items_from(test_items);

    assert_that(cgreen_vector_size(test_items) == 0);
}

Ensure(Runner, can_sort_a_list_of_a_single_tests) {
    TestItem test_item = {
        (char *)"", (char *)"Context1", (char *)"Test1",
    };
    CgreenVector *test_items = create_cgreen_vector(NULL);
    cgreen_vector_add(test_items, &test_item);

    test_items = sorted_test_items_from(test_items);
    assert_that(((TestItem *)cgreen_vector_get(test_items, 0))->test_name,
                is_equal_to_string("Test1"));
}

Ensure(Runner, can_sort_a_list_of_two_unordered_tests) {
    TestItem test_items[] = {
        {(char *)"", (char *)"Context1", (char *)"Test2"},
        {(char *)"", (char *)"Context1", (char *)"Test1"},
    };

    CgreenVector *tests = create_cgreen_vector(NULL);
    add_test_items_to_vector(test_items, tests, 2);

    tests = sorted_test_items_from(tests);

    assert_that(((TestItem *)cgreen_vector_get(tests, 0))->test_name,
                is_equal_to_string("Test1"));
    assert_that(((TestItem *)cgreen_vector_get(tests, 1))->test_name,
                is_equal_to_string("Test2"));
}

Ensure(Runner, can_sort_an_ordered_list_of_two_tests) {
    TestItem test_item[] = {
        {(char *)"", (char *)"Context1", (char *)"Test1"},
        {(char *)"", (char *)"Context1", (char *)"Test2"}
    };

    CgreenVector *test_items = create_cgreen_vector(NULL);
    cgreen_vector_add(test_items, &test_item[0]);
    cgreen_vector_add(test_items, &test_item[1]);

    test_items = sorted_test_items_from(test_items);

    assert_that(((TestItem *)cgreen_vector_get(test_items, 0))->test_name,
                is_equal_to_string("Test1"));
    assert_that(((TestItem *)cgreen_vector_get(test_items, 1))->test_name,
                is_equal_to_string("Test2"));
}

Ensure(Runner, can_sort_an_unordered_list_of_tests) {
    TestItem unordered_test_items[] = {
        {(char *)"", (char *)"Context1", (char *)"Test9"},
        {(char *)"", (char *)"Context1", (char *)"Test6"},
        {(char *)"", (char *)"Context1", (char *)"Test3"},
        {(char *)"", (char *)"Context1", (char *)"Test1"},
        {(char *)"", (char *)"Context1", (char *)"Test5"},
        {(char *)"", (char *)"Context1", (char *)"Test8"},
        {(char *)"", (char *)"Context1", (char *)"Test7"},
        {(char *)"", (char *)"Context1", (char *)"Test4"},
        {(char *)"", (char *)"Context1", (char *)"Test2"},
    };
    const char *expected_test_name[] = {
        "Test1", "Test2", "Test3", "Test4", "Test5", "Test6", "Test7", "Test8", "Test9" };

    CgreenVector *tests = create_cgreen_vector(NULL);
    add_test_items_to_vector(unordered_test_items, tests,
                             sizeof(unordered_test_items)/sizeof(unordered_test_items[0]));

    tests = sorted_test_items_from(tests);

    for (int i=0; i<cgreen_vector_size(tests); i++)
        assert_that(((TestItem *)cgreen_vector_get(tests, i))->test_name,
                    is_equal_to_string(expected_test_name[i]));
}

/* vim: set ts=4 sw=4 et cindent: */
/* Local variables: */
/* tab-width: 4     */
/* End:             */
