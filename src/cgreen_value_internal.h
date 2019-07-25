#ifndef CGREEN_VALUE_INTERNAL_H
#define CGREEN_VALUE_INTERNAL_H

#include <cgreen/cgreen_value.h>

/* CgreenValues are used from some user level tests so must be compilable in C++ */
#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

extern CgreenValue *create_cgreen_value(CgreenValue value);
extern CgreenValue make_cgreen_integer_value(intptr_t integer);
extern CgreenValue make_cgreen_string_value(const char *string);
extern CgreenValue make_cgreen_double_value(double value);
extern CgreenValue make_cgreen_pointer_value(void *ptr);
extern CgreenValue make_cgreen_by_value(void *pointer, size_t size);
extern void destroy_cgreen_value(CgreenValue value);

#ifdef __cplusplus
    }
}
#endif

#endif
