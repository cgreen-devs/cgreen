#include "../cgreen.h"
#include <stdlib.h>
#include <stdio.h>

void these_should_be_true() {
    assert_true(1, "One is true");
    assert_false(1, "One is not false");
}

void these_should_be_false() {
    assert_true(0, "Nought is not true");
    assert_false(0, "Nought is false");
}

void these_should_be_equal() {
    assert_equal(1, 1, "%d should be %d", 1, 1);
    assert_not_equal(1, 1, "%d should not be %d", 1, 1);
}

void these_should_not_be_equal() {
    assert_equal(0, 1, "%d should be %d", 0, 1);
    assert_not_equal(0, 1, "%d should not be %d", 0, 1);
}

void these_strings_should_match() {
    assert_string_equal("Hello", "Hello", "%s should be %s", "Hello", "Hello");
    assert_string_not_equal("Hello", "Hello", "%s should not be %s", "Hello", "Hello");
}

void these_strings_should_not_match() {
    assert_string_equal("Hello", "hello", "%s should be %s", "Hello", "hello");
    assert_string_not_equal("Hello", "hello", "%s should not be %s", "Hello", "hello");
}

TestSuite *assertion_tests() {
    TestSuite *suite = create_named_test_suite("assertion tests");
    add_unit_test(suite, these_should_be_true);
    add_unit_test(suite, these_should_be_false);
    add_unit_test(suite, these_should_be_equal);
    add_unit_test(suite, these_should_not_be_equal);
    add_unit_test(suite, these_strings_should_match);
    add_unit_test(suite, these_strings_should_not_match);
    return suite;
}

static int an_integer = 0;

void set_up_an_integer() {
    an_integer = 1;
}

void confirm_integer_is_set_up() {
    assert_equal(an_integer, 1, "Could not set up the integer");
    an_integer = 2;
}

void check_again_during_teardown() {
    assert_equal(an_integer, 1, "Integer was changed from %d to %d", 1, an_integer);
}

TestSuite *fixture_tests() {
    TestSuite *suite = create_named_test_suite("fixture tests");
    setup(suite, set_up_an_integer);
    teardown(suite, check_again_during_teardown);
    add_unit_test(suite, confirm_integer_is_set_up);
    return suite;
}

void print_something_during_setup() {
    printf("\tI was called during setup\n");
}

void print_something_during_a_test() {
    printf("\tI am a test\n");
}

void print_something_during_teardown() {
    printf("\tI was called during teardown\n");
}

TestSuite *visible_test() {
    TestSuite *suite = create_named_test_suite("visible test");
    setup(suite, print_something_during_setup);
    add_unit_test(suite, print_something_during_a_test);
    teardown(suite, print_something_during_teardown);
    return suite;
}

void print_something_during_suite_setup() {
    printf("I was called during suite setup\n");
}

void print_something_during_suite_teardown() {
    printf("I was called during suite teardown\n");
}

TestSuite *visible_fixtures() {
    TestSuite *suite = create_named_test_suite("visible fixtures");
    setup(suite, print_something_during_suite_setup);
    add_suite(suite, visible_test());
    add_suite(suite, visible_test());
    teardown(suite, print_something_during_suite_teardown);
    return suite;
}

int interference = 0;

void create_test_interference() {
    interference = 1;
}

void prove_there_is_no_test_interference() {
    assert_equal(interference, 0, "Interference detected as %d", interference);
}

void seg_fault() {
    int *p = NULL;
    (*p)++;
}

void time_out_in_only_one_second() {
    stop_soon(1);
    sleep(10);
}

TestSuite *isolation_tests() {
    TestSuite *suite = create_named_test_suite("isolation tests");
    add_unit_test(suite, create_test_interference);
    add_unit_test(suite, prove_there_is_no_test_interference);
    add_unit_test(suite, seg_fault);
    add_unit_test(suite, time_out_in_only_one_second);
    return suite;
}

int give_return_sequence() {
    return integer_sequence(1, 2, 3, 4, 5, 6);
}

void confirm_every_value_in_sequence() {
    assert_equal(give_return_sequence(), 1, "Expecting 1");
    assert_equal(give_return_sequence(), 2, "Expecting 2");
    assert_equal(give_return_sequence(), 3, "Expecting 3");
}

void show_that_the_sequence_restarts_across_tests() {
    assert_equal(give_return_sequence(), 1, "Expecting 1");
    assert_equal(give_return_sequence(), 2, "Expecting 2");
    assert_equal(give_return_sequence(), 3, "Expecting 3");
}

char *give_string_sequence() {
    return string_sequence("Hello", "Goodbye");
}

void confirm_every_string_in_sequence() {
    assert_string_equal(give_string_sequence(), "Hello", "Expecting Hello");
    assert_string_equal(give_string_sequence(), "Goodbye", "Expecting Goodbye");
}

void mock_callback(char *string, int integer, void *address) {
    assert_string_equal(string, string_sequence("Hello", "Goodbye"), "Got [%s]", string);
    assert_equal(integer, integer_sequence(1, 2), "Got [%d]", integer);
    assert_equal(address, address_sequence(NULL, (void *)1), "Got [%p]", address);
}

void feed_the_mock_callback_with_correct_arguments() {
    mock_callback("Hello", 1, NULL);
    mock_callback("Goodbye", 2, (void *)1);
}

void dont_call_me() {
    called_never();
}

void call_me_once() {
    called_once();
}

void call_me_twice() {
    expected_call_count(2);
}

void trip_the_counters() {
    //dont_call_me();
    call_me_once();
    call_me_twice();
    call_me_twice();
}

TestSuite *sequence_tests() {
    TestSuite *suite = create_named_test_suite("sequence tests");
    add_unit_test(suite, confirm_every_value_in_sequence);
    add_unit_test(suite, show_that_the_sequence_restarts_across_tests);
    add_unit_test(suite, confirm_every_string_in_sequence);
    add_unit_test(suite, feed_the_mock_callback_with_correct_arguments);
    add_unit_test(suite, trip_the_counters);
    return suite;
}

void take_so_long_that_ctrl_c_is_needed() {
    sleep(10);
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, assertion_tests());
    add_suite(suite, fixture_tests());
    add_suite(suite, visible_fixtures());
    add_suite(suite, isolation_tests());
    add_suite(suite, sequence_tests());
    //add_unit_test(suite, take_so_long_that_ctrl_c_is_needed);
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
