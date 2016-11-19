#ifndef CGREEN_VALUE_HEADER
#define CGREEN_VALUE_HEADER

#include <stdint.h>

typedef enum {INTEGER, STRING, DOUBLE, POINTER} CgreenValueType;

typedef struct {
    CgreenValueType type;
    union {
        intptr_t integer_value;
        double double_value;
        void *pointer_value;
        const char *string_value;
    } value;
} CgreenValue;

#ifdef __cplusplus
namespace cgreen {
#endif

extern CgreenValue make_cgreen_integer_value(intptr_t integer);
extern CgreenValue make_cgreen_string_value(const char *string);
extern CgreenValue make_cgreen_double_value(double value);
extern CgreenValue make_cgreen_pointer_value(void *ptr);

#ifdef __cplusplus
} // namespace cgreen
#endif
 
#endif
