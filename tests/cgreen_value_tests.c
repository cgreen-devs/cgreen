#include <cgreen/cgreen.h>
#include <include/cgreen/cgreen_value.h>

#include "cgreen_value_internal.h"

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(CgreenValue);
BeforeEach(CgreenValue) {}
AfterEach(CgreenValue) {}

Ensure(CgreenValue, makes_integer_value) {
    CgreenValue value = make_cgreen_integer_value(14);
    assert_that(value.type, is_equal_to(INTEGER));
    assert_that(value.value.integer_value, is_equal_to(14));
    destroy_cgreen_value(value);
}

Ensure(CgreenValue, makes_string_value) {
    CgreenValue value = make_cgreen_string_value("hej hopp");
    assert_that(value.type, is_equal_to(STRING));
    assert_that(value.value.string_value, is_equal_to_string("hej hopp"));
    destroy_cgreen_value(value);
}

Ensure(CgreenValue, makes_pointer_value) {
    int buffer[20];
    CgreenValue value = make_cgreen_pointer_value(buffer);
    assert_that(value.type, is_equal_to(POINTER));
    assert_that(value.value.string_value, is_equal_to(buffer));
    destroy_cgreen_value(value);
}

Ensure(CgreenValue, makes_double_value) {
    CgreenValue value = make_cgreen_double_value(3.1415926);
    assert_that(value.type, is_equal_to(DOUBLE));
    assert_that_double(value.value.double_value, is_equal_to_double(3.1415926));
    destroy_cgreen_value(value);
}

Ensure(CgreenValue, makes_by_value) {
    char actualString[] = "1234";
    CgreenValue value = make_cgreen_by_value(actualString, 4);
    assert_that(value.type, is_equal_to(BYVALUE));
    assert_that(value.value.pointer_value, is_equal_to_string("1234"));
}

TestSuite *cgreen_value_tests(void) {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, CgreenValue, makes_integer_value);
    add_test_with_context(suite, CgreenValue, makes_string_value);
    add_test_with_context(suite, CgreenValue, makes_pointer_value);
    add_test_with_context(suite, CgreenValue, makes_double_value);
    add_test_with_context(suite, CgreenValue, makes_by_value);

    return suite;
}
