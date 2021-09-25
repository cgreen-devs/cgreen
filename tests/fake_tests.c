#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/vector.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

// ################# SUITE SETUP ####################################

Describe(Fake);
BeforeEach(Fake) {}
AfterEach(Fake) {}

// ################# MOCK FUNCTIONS #################################

static int function_mock(int param1, char *param2) {
    return (int)mock(param1, param2);
}

// ################# FAKE IMPLEMENTATIONS ###########################

static void _fake_return_1_when_param1_is_0(const CgreenVector *const params, CgreenValue *const return_value) {
    memset(return_value, 0, sizeof(CgreenValue));
    return_value->type = CGREEN_INTEGER;

    int param1 = (int)(((CgreenValue *)cgreen_vector_get(params, 0))->value.integer_value);

    if (param1 == 0) {
        return_value->value.integer_value = 1;
    }
}

static void _fake_return_param2(const CgreenVector *const params, CgreenValue *const return_value) {
    memset(return_value, 0, sizeof(CgreenValue));
    return_value->type = CGREEN_STRING;

    const char *param2 = ((CgreenValue *)cgreen_vector_get(params, 1))->value.string_value;
    return_value->value.string_value = param2;
    return_value->value_size = strlen(param2);
}

static int _fake_capture;

static void _fake_capture_param1(const CgreenVector *const params, CgreenValue *const return_value) {
    memset(return_value, 0, sizeof(CgreenValue));

    _fake_capture = ((CgreenValue *)cgreen_vector_get(params, 0))->value.integer_value;
}

static void _fake_return_captured(const CgreenVector *const params, CgreenValue *const return_value) {
    (void)params;
    
    memset(return_value, 0, sizeof(CgreenValue));
    return_value->type = CGREEN_INTEGER;

    return_value->value.integer_value = _fake_capture;
}

static void _fake_compare_params(const CgreenVector *const params, CgreenValue *const return_value) {
    memset(return_value, 0, sizeof(CgreenValue));
    return_value->type = CGREEN_INTEGER;

    int param1 = (int)(((CgreenValue *)cgreen_vector_get(params, 0))->value.integer_value);
    const char *param2 = ((CgreenValue *)cgreen_vector_get(params, 1))->value.string_value;

    int param2_int = atoi(param2);

    return_value->value.integer_value = (param1 == param2_int);
}

// ################# TEST CASES #####################################

Ensure(Fake, conditional_return_values) {
    // Arrange
    char *dont_care = strdup("");

    // Expect
    always_expect(function_mock, with_fake_implementation(&_fake_return_1_when_param1_is_0));

    // Act & Assert
    assert_that(function_mock(0, dont_care), is_equal_to(1));
    assert_that(function_mock(1, dont_care), is_equal_to(0));
    assert_that(function_mock(-1, dont_care), is_equal_to(0));
}

Ensure(Fake, retruns_parameter_contents) {
    // Arrange
    char *test_string = strdup("TEST");
    char *empty_string = strdup("");

    // Expect
    always_expect(function_mock, with_fake_implementation(&_fake_return_param2));

    // Act & Assert
    assert_that(function_mock(0, test_string), is_equal_to_string(test_string));
    assert_that(function_mock(0, empty_string), is_equal_to_string(empty_string));
}

Ensure(Fake, capture_value_for_return_in_other_mock) {
    // Arrange

    // Expect
    expect(function_mock, with_fake_implementation(&_fake_capture_param1));
    expect(function_mock, with_fake_implementation(&_fake_return_captured));

    // Act & Assert
    assert_that(function_mock(42, NULL), is_equal_to(0));
    assert_that(function_mock(0, NULL), is_equal_to(42));
}

Ensure(Fake, compare_params_with_each_other) {
    // Arrange
    int test_int = 42;
    char *test_string = strdup("42");

    // Expect
    expect(function_mock, with_fake_implementation(&_fake_compare_params));

    // Act & Assert
    assert_that(function_mock(test_int, test_string), is_equal_to(1));
}

// ################# SUITE SETUP ####################################

TestSuite *fake_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Fake, conditional_return_values);
    add_test_with_context(suite, Fake, retruns_parameter_contents);
    add_test_with_context(suite, Fake, capture_value_for_return_in_other_mock);
    add_test_with_context(suite, Fake, compare_params_with_each_other);

    return suite;
}
