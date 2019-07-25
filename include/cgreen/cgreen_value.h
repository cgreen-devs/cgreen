#ifndef CGREEN_VALUE_HEADER
#define CGREEN_VALUE_HEADER

#include <stdint.h>
#include <stddef.h>

typedef enum {INTEGER, STRING, DOUBLE, POINTER, BYVALUE} CgreenValueType;

typedef struct {
    CgreenValueType type;
    union {
        intptr_t integer_value;
        double double_value;
        void *pointer_value;
        const char *string_value;
    } value;
    size_t value_size;
} CgreenValue;

#endif
