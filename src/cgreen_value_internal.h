#ifndef CGREEN_VALUE_INTERNAL_H
#define CGREEN_VALUE_INTERNAL_H

#include <cgreen/cgreen_value.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

extern CgreenValue *create_cgreen_value(CgreenValue value);
extern CgreenValue make_cgreen_integer_value(intptr_t integer);
extern CgreenValue make_cgreen_string_value(const char *string);
extern CgreenValue make_cgreen_double_value(double value);
extern CgreenValue make_cgreen_pointer_value(void *ptr);

#ifdef __cplusplus
    }
}
#endif

#endif
