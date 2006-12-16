#include "../cgreen.h"
#include <stdlib.h>
#include <stdio.h>

void these_should_be_true() {
    assert_true(1);
    assert_false(1);
}

void these_should_be_false() {
    assert_true(0);
    assert_false(0);
}

void these_should_be_equal() {
    assert_equal(1, 1);
    assert_not_equal(1, 1);
}

void these_should_not_be_equal() {
    assert_equal(0, 1);
    assert_not_equal(0, 1);
}

void these_strings_should_match() {
    assert_string_equal("Hello", "Hello");
    assert_string_not_equal("Hello", "Hello");
}

void these_strings_should_not_match() {
    assert_string_equal("Hello", "hello");
    assert_string_not_equal("Hello", "hello");
}

TestSuite *assertion_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, these_should_be_true);
    add_test(suite, these_should_be_false);
    add_test(suite, these_should_be_equal);
    add_test(suite, these_should_not_be_equal);
    add_test(suite, these_strings_should_match);
    add_test(suite, these_strings_should_not_match);
    return suite;
}

static int an_integer = 0;

void set_up_an_integer() {
    an_integer = 1;
}

void confirm_integer_is_set_up() {
    assert_equal_with_message(an_integer, 1, "Could not set up the integer");
    an_integer = 2;
}

void check_again_during_teardown() {
    assert_equal_with_message(an_integer, 1, "Integer was changed from 1 to %d", an_integer);
}

TestSuite *fixture_tests() {
    TestSuite *suite = create_test_suite();
    setup(suite, set_up_an_integer);
    teardown(suite, check_again_during_teardown);
    add_test(suite, confirm_integer_is_set_up);
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
    TestSuite *suite = create_test_suite();
    setup(suite, print_something_during_setup);
    add_test(suite, print_something_during_a_test);
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
    TestSuite *suite = create_test_suite();
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
    assert_equal(interference, 0);
}

void seg_fault() {
    int *p = NULL;
    (*p)++;
}

void time_out_in_only_one_second() {
    die_in(1);
    sleep(10);
}

TestSuite *isolation_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, create_test_interference);
    add_test(suite, prove_there_is_no_test_interference);
    add_test(suite, seg_fault);
    add_test(suite, time_out_in_only_one_second);
    return suite;
}

static void takes_integer(int i) {
    mock(i);
}

static void expectation_confirmed() {
    expect(takes_integer, want(i, 3));
    takes_integer(3);
}

static void expectation_dashed() {
    expect(takes_integer, want(i, 3));
    takes_integer(4);
}
static void mixed_parameters(int i, char *s) {
    mock(i, s);
}

static void confirming_multiple_parameters_multiple_times() {
    expect(mixed_parameters, want(i, 1), want_string(s, "Hello"));
    expect(mixed_parameters, want(i, 2), want_string(s, "Goodbye"));
    mixed_parameters(1, "Hello");
    mixed_parameters(2, "Goodbye");
}

static void breaking_multiple_parameters_multiple_times() {
    expect(mixed_parameters, want(i, 1), want_string(s, "Hello"));
    expect(mixed_parameters, want(i, 2), want_string(s, "Goodbye"));
    mixed_parameters(10, "Helloo");
    mixed_parameters(20, "Gooodbye");
}

static void uncalled_expectations_should_throw_errors() {
    expect(mixed_parameters, want(i, 1), want_string(s, "Hello"));
}

static void unexpected_call_should_throw_error() {
    expect_never(mixed_parameters);
    mixed_parameters(10, "Helloo");
}

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, expectation_confirmed);
    add_test(suite, expectation_dashed);
    add_test(suite, confirming_multiple_parameters_multiple_times);
    add_test(suite, breaking_multiple_parameters_multiple_times);
    add_test(suite, uncalled_expectations_should_throw_errors);
    add_test(suite, unexpected_call_should_throw_error);
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
    add_suite(suite, mock_tests());
    //add_unit_test(suite, take_so_long_that_ctrl_c_is_needed);
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
