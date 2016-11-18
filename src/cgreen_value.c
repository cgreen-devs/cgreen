#include <cgreen/cgreen_value.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
namespace cgreen {
#endif

    CgreenValue make_cgreen_integer_value(intptr_t integer) {
        CgreenValue value = {INTEGER, {integer}};
        value.value.integer_value = integer;
        return value;
    }

    CgreenValue make_cgreen_string_value(const char *string) {
        CgreenValue value = {STRING, {string}};
        value.value.string_value = string;
        return value;
    }
    
    CgreenValue make_cgreen_pointer_value(void *pointer) {
        CgreenValue value = {POINTER, {pointer}};
        value.value.pointer_value = pointer;
        return value;
    }

    CgreenValue make_cgreen_double_value(double d) {
        CgreenValue value = {DOUBLE, {d}};
        value.value.double_value = d;
        return value;
    }


#ifdef __cplusplus
} // namespace cgreen
#endif
