#include <cgreen/cgreen.h>

#include "include/cgreen/cgreen_value.h"

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
}

Ensure(CgreenValue, makes_string_value) {
    CgreenValue value = make_cgreen_string_value("hej hopp");
    assert_that(value.type, is_equal_to(STRING));
    assert_that(value.value.string_value, is_equal_to_string("hej hopp"));
}

Ensure(CgreenValue, makes_pointer_value) {
    int buffer[20];
    CgreenValue value = make_cgreen_pointer_value(buffer);
    assert_that(value.type, is_equal_to(POINTER));
    assert_that(value.value.string_value, is_equal_to(buffer));
}

Ensure(CgreenValue, makes_double_value) {
    CgreenValue value = make_cgreen_double_value(3.1415926);
    assert_that(value.type, is_equal_to(DOUBLE));
    assert_that(value.value.double_value, is_equal_to(3.1415926));
}
