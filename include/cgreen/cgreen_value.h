#ifndef CGREEN_VALUE_HEADER
#define CGREEN_VALUE_HEADER

#include <stdint.h>
#include <stddef.h>

typedef enum {
              CGREEN_INTEGER,
              CGREEN_STRING,
              CGREEN_DOUBLE,
              CGREEN_POINTER,
              CGREEN_BYVALUE,
              CGREEN_BYREFERENCE
} CgreenValueType;

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
