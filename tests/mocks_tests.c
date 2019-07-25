#include <cgreen/boxed_double.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/unit.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(Mocks);
BeforeEach(Mocks) {}
AfterEach(Mocks) {}

static int integer_out(void) {
    return (int)mock();
}

Ensure(Mocks, default_return_value_when_no_presets_for_loose_mock) {
    cgreen_mocks_are(loose_mocks);
    assert_that(integer_out(), is_equal_to(0));
}

Ensure(Mocks, can_stub_an_integer_return) {
    expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(3));
}

Ensure(Mocks, repeats_return_value_when_set_to_always) {
    always_expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(3));
    assert_that(integer_out(), is_equal_to(3));
}

Ensure(Mocks, can_stub_an_integer_return_sequence) {
    expect(integer_out,
        will_return(1)
    );

    expect(integer_out,
        will_return(2)
    );

    expect(integer_out,
        will_return(3)
    );

    assert_that(integer_out(), is_equal_to(1));
    assert_that(integer_out(), is_equal_to(2));
    assert_that(integer_out(), is_equal_to(3));
}

Ensure(Mocks, expectations_are_reset_between_tests_with_loose_mocks) {
    cgreen_mocks_are(loose_mocks);
    assert_that(integer_out(), is_equal_to(0));
}

static char *string_out(int p1) {
    return (char *)mock(p1);
}

Ensure(Mocks, can_stub_a_string_return) {
    expect(string_out, will_return("hello"));
    assert_that(string_out(1), is_equal_to_string("hello"));
}

Ensure(Mocks, can_stub_a_string_sequence) {
    expect(string_out, will_return("hello"));
    expect(string_out, will_return("goodbye"));

    assert_that(string_out(2), is_equal_to_string("hello"));
    assert_that(string_out(3), is_equal_to_string("goodbye"));
}

static void integer_in(int i) {
    mock(i);
}

Ensure(Mocks, expecting_once_with_any_parameters) {
    expect(integer_in);
    integer_in(3);
}

Ensure(Mocks, expecting_once_with_parameter_checks_that_parameter) {
    expect(integer_in, when(i, is_equal_to(3)));
    integer_in(3);
}

Ensure(Mocks, always_expect_keeps_affirming_parameter) {
    always_expect(integer_in, when(i, is_equal_to(3)));
    integer_in(3);
    integer_in(3);
    integer_in(3);
}

Ensure(Mocks, expect_a_sequence) {
    expect(integer_in, when(i, is_equal_to(1)));
    expect(integer_in, when(i, is_equal_to(2)));
    expect(integer_in, when(i, is_equal_to(3)));
    integer_in(1);
    integer_in(2);
    integer_in(3);
}

static void string_in(const char *s) {
    mock(s);
}

Ensure(Mocks, string_expect_is_confirmed) {
    expect(string_in, when(s, is_equal_to_string("hello")));
    string_in("hello");
}

Ensure(Mocks, string_contains_expectation_is_confirmed) {
    expect(string_in, when(s, contains_string("hello")));
    string_in("alice, hello");
}

Ensure(Mocks, string_expect_is_confirmed_even_when_null) {
    expect(string_in, when(s, is_equal_to_string((char *)NULL)));
    string_in(NULL);
}

Ensure(Mocks, string_expect_sequence) {
    expect(string_in, when(s, is_equal_to_string("hello")));
    expect(string_in, when(s, is_equal_to_string("goodbye")));
    string_in("hello");
    string_in("goodbye");
}

Ensure(Mocks, expecting_once_with_non_null_parameter_checks_that_parameter) {
    expect(string_in, when(s, is_non_null));
    string_in("anything");
}

static void double_in(double d) {
    mock(box_double(d));
}

Ensure(Mocks, double_expect_is_confirmed) {
    expect(double_in, when(d, is_equal_to_double(3.14)));
    double_in(3.14);
}

Ensure(Mocks, double_expect_sequence) {
    expect(double_in, when(d, is_equal_to_double(1.0)));
    expect(double_in, when(d, is_equal_to_double(2.0)));
    double_in(1.0);
    double_in(2.0);
}


/* non-static to avoid "unused" warning in C++ as long as
   'can_always_return_double' does not work in C++ */
double double_out(void) {
    /* TODO: For v2 this should change to "return mock_double();" */
    return unbox_double(mock());
}

#ifndef __cplusplus
/* TODO: This does not work in C++ because of the overloading of
   assert_that() which currently has no match for double */
Ensure(Mocks, can_return_double) {
    expect(double_out, will_return_double(4.23));
    assert_that_double(double_out(), is_equal_to_double(4.23));
}

Ensure(Mocks, can_always_return_double) {
    always_expect(double_out, will_return_double(4.23));
    assert_that_double(double_out(), is_equal_to_double(4.23));
    assert_that_double(double_out(), is_equal_to_double(4.23));
    assert_that_double(double_out(), is_equal_to_double(4.23));
}
#endif


typedef struct {
    int field;
} S;

/* The mock function */
S func(void) {
    S s;
    S *sp;

    sp = (S *)mock();

    /* Copy content of struct pointed to by returned value */
    memcpy(&s, sp, sizeof(S));

    /* Return it */
    return s;
}

Ensure(Mocks, can_return_struct) {
    /* Allocate a struct */
    S *sp = (S *)malloc(sizeof(S));

    /* Prime one field */
    sp->field = 42;

    /* Expect a call and return a pointer to the struct */
    expect(func, will_return(sp));

    /* Call our mock function which returns a struct */
    S ret = func();

    /* And assert that the field in the returned struct is propagated */
    assert_that(ret.field, is_equal_to(42));

    free(sp);
}

static void mixed_parameters(int i, const char *s) {
    mock(i, s);
}

Ensure(Mocks, confirming_multiple_parameters_multiple_times) {
    expect(mixed_parameters,
        when(i, is_equal_to(1)),
        when(s, is_equal_to_string("Hello"))
    );
    expect(mixed_parameters,
        when(i, is_equal_to(2)),
        when(s, is_equal_to_string("Goodbye"))
    );

    mixed_parameters(1, "Hello");
    mixed_parameters(2, "Goodbye");
}

static int sample_mock(int i, const char *s) {
    return (int)mock(i, s);
}

Ensure(Mocks, can_mock_full_function_call) {
    expect(sample_mock,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );

    assert_that(sample_mock(666, "devil"), is_equal_to(5));
}

Ensure(Mocks, when_called_with_always_should_not_tally_counts) {
    always_expect(string_out,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );
}

Ensure(Mocks, can_mock_full_sequence) {
    expect(sample_mock,
       will_return(5),
       when(i, is_equal_to(666)),
       when(s, is_equal_to_string("devil"))
    );

    expect(sample_mock,
        will_return(6),
        when(i, is_equal_to(667)),
        when(s, is_equal_to_string("beastie"))
    );

    assert_that(sample_mock(666, "devil"), is_equal_to(5));
    assert_that(sample_mock(667, "beastie"), is_equal_to(6));
}

Ensure(Mocks, can_always_mock_full_function_call) {
    always_expect(sample_mock,
        will_return(5),
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );

    assert_that(sample_mock(666, "devil"), is_equal_to(5));
    assert_that(sample_mock(666, "devil"), is_equal_to(5));
    assert_that(sample_mock(666, "devil"), is_equal_to(5));
}

static void sample_mock_with_parameters_without_space_between_them(int i, const char *s) {
    mock(i,s);
}

Ensure(Mocks, can_mock_full_function_call_when_there_is_no_space_between_parameters) {
    expect(sample_mock_with_parameters_without_space_between_them,
        when(i, is_equal_to(666)),
        when(s, is_equal_to_string("devil"))
    );

    sample_mock_with_parameters_without_space_between_them(666, "devil");
}

typedef struct {
    double a;
    double b;
    const char *name;
} LargerThanIntptr;

static void out_param_mock(LargerThanIntptr* result) {
    mock(result);
}


Ensure(Mocks, can_stub_an_out_parameter) {
    LargerThanIntptr actual = { 3.14, 6.66, "bob" };
    LargerThanIntptr local = { 4.13, 7.89, "alice" };

    expect(out_param_mock,
        will_set_contents_of_parameter(result, &actual, sizeof(LargerThanIntptr))
    );

    out_param_mock(&local);
    assert_that_double(actual.a, is_equal_to_double(local.a));
    assert_that_double(actual.b, is_equal_to_double(local.b));
    assert_that(actual.name, is_equal_to_string(local.name));
    assert_that(&local, is_equal_to_contents_of(&actual, sizeof(LargerThanIntptr)));
}

// function which when mocked will be referred to by preprocessor macro
static void function_macro_mock(void) {
    mock();
}

#define FUNCTION_MACRO function_macro_mock

Ensure(Mocks, can_mock_a_function_macro) {

    // expect to mock by real function name
    expect(function_macro_mock);

    function_macro_mock();

    // expect to mock by macro function name
    expect(FUNCTION_MACRO);

    FUNCTION_MACRO();
}

#undef FUNCTION_MACRO

static void simple_mocked_function(int first, int second) {
    mock(first, second);
}

Ensure(Mocks, constraint_number_of_calls_when_no_when_is_present) {
    expect(simple_mocked_function, times(2));
    simple_mocked_function(1, 2);
    simple_mocked_function(1, 2);
}

Ensure(Mocks, constraint_number_of_calls_when_is_present) {
    expect(simple_mocked_function, when(first, is_equal_to(1)), times(2));
    simple_mocked_function(1, 2);
    simple_mocked_function(1, 2);
}

Ensure(Mocks, constraint_number_of_calls_when_multiple_expectations_are_present) {
    expect(simple_mocked_function, when(first, is_equal_to(1)), times(2));
    expect(simple_mocked_function, when(first, is_equal_to(2)), times(1));
    simple_mocked_function(1, 2);
    simple_mocked_function(1, 2);
    simple_mocked_function(2, 2);
}

Ensure(Mocks, constraint_number_of_calls_order_of_expectations_matter) {
    expect(simple_mocked_function, when(first, is_equal_to(1)), times(1));
    expect(simple_mocked_function, when(first, is_equal_to(2)), times(1));
    expect(simple_mocked_function, when(first, is_equal_to(1)), times(1));
    simple_mocked_function(1, 2);
    simple_mocked_function(2, 2);
    simple_mocked_function(1, 2);
}

static int sideeffect_changed = 1;
static int mock_with_side_effect(void) {
    return (int)mock();
}
static void the_sideeffect(void * data) {
    assert_that(*(int*)data, is_equal_to(99));
    sideeffect_changed = 2;
}

Ensure(Mocks, mock_expect_with_side_effect) {
    int data_passed_to_sideeffect = 99;
    expect(mock_with_side_effect,
           with_side_effect(&the_sideeffect, &data_passed_to_sideeffect),
           will_return(22));

    assert_that(mock_with_side_effect(), is_equal_to(22));

    assert_that(sideeffect_changed, is_equal_to(2));
}


typedef struct Box {
    int height;
    int weight;
} Box;

Box retrieveBox(void) {
    return *(Box *)mock();
}

Ensure(Mocks, can_return_by_value) {
    Box someBox = {.height = 10, .weight = 20};
    expect(retrieveBox, will_return_by_value(someBox, sizeof(Box)));
    someBox.height = 30;

    Box actualBox = retrieveBox();
    assert_that(actualBox.weight, is_equal_to(20));
    assert_that(actualBox.height, is_equal_to(10));
}

Box retrieveSpecialBox(int boxNumber) {
    return *(Box *)mock(boxNumber);
}

Ensure(Mocks, can_return_by_value_depending_on_input_parameter) {
    Box box1 = {.height = 10, .weight = 20};
    Box box2 = {.height = 5, .weight = 33};
    expect(retrieveSpecialBox, will_return_by_value(box1, sizeof(Box)), when(boxNumber, is_equal_to(1)));
    expect(retrieveSpecialBox, will_return_by_value(box2, sizeof(Box)), when(boxNumber, is_equal_to(2)));
    box1.height = 30;

    Box retrievedBox1 = retrieveSpecialBox(1);
    assert_that(retrievedBox1.weight, is_equal_to(20));
    assert_that(retrievedBox1.height, is_equal_to(10));
    Box retrievedBox2 = retrieveSpecialBox(2);
    assert_that(retrievedBox2.weight, is_equal_to(33));
    assert_that(retrievedBox2.height, is_equal_to(5));
}


TestSuite *mock_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Mocks, default_return_value_when_no_presets_for_loose_mock);
    add_test_with_context(suite, Mocks, can_stub_an_integer_return);
    add_test_with_context(suite, Mocks, repeats_return_value_when_set_to_always);
    add_test_with_context(suite, Mocks, can_stub_an_integer_return_sequence);
    add_test_with_context(suite, Mocks, expectations_are_reset_between_tests_with_loose_mocks);
    add_test_with_context(suite, Mocks, can_stub_a_string_return);
    add_test_with_context(suite, Mocks, can_stub_a_string_sequence);
    add_test_with_context(suite, Mocks, expecting_once_with_any_parameters);
    add_test_with_context(suite, Mocks, expecting_once_with_parameter_checks_that_parameter);
    add_test_with_context(suite, Mocks, always_expect_keeps_affirming_parameter);
    add_test_with_context(suite, Mocks, expect_a_sequence);
    add_test_with_context(suite, Mocks, string_expect_is_confirmed);
    add_test_with_context(suite, Mocks, string_expect_is_confirmed_even_when_null);
    add_test_with_context(suite, Mocks, string_expect_sequence);
    add_test_with_context(suite, Mocks, expecting_once_with_non_null_parameter_checks_that_parameter);
    add_test_with_context(suite, Mocks, double_expect_is_confirmed);
    add_test_with_context(suite, Mocks, double_expect_sequence);
    add_test_with_context(suite, Mocks, confirming_multiple_parameters_multiple_times);
    add_test_with_context(suite, Mocks, can_mock_full_function_call);
    add_test_with_context(suite, Mocks, when_called_with_always_should_not_tally_counts);
    add_test_with_context(suite, Mocks, can_mock_full_sequence);
    add_test_with_context(suite, Mocks, can_always_mock_full_function_call);
    add_test_with_context(suite, Mocks, can_mock_full_function_call_when_there_is_no_space_between_parameters);
    add_test_with_context(suite, Mocks, can_stub_an_out_parameter);
    add_test_with_context(suite, Mocks, string_contains_expectation_is_confirmed);
    add_test_with_context(suite, Mocks, can_mock_a_function_macro);
    add_test_with_context(suite, Mocks, constraint_number_of_calls_when_no_when_is_present);
    add_test_with_context(suite, Mocks, constraint_number_of_calls_when_is_present);
    add_test_with_context(suite, Mocks, constraint_number_of_calls_when_multiple_expectations_are_present);
    add_test_with_context(suite, Mocks, constraint_number_of_calls_order_of_expectations_matter);
    add_test_with_context(suite, Mocks, mock_expect_with_side_effect);
    add_test_with_context(suite, Mocks, can_return_by_value);
    add_test_with_context(suite, Mocks, can_return_by_value_depending_on_input_parameter);

    return suite;
}
