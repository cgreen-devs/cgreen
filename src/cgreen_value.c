#include <cgreen/cgreen_value.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <include/cgreen/cgreen_value.h>

static char *stringdup(const char *string) {
    if (string == NULL)
        return NULL;
    else
        return strcpy((char *)malloc(strlen(string)+1), string);
}

CgreenValue *create_cgreen_value(CgreenValue value) {
    CgreenValue *ptr = (CgreenValue*)malloc(sizeof(CgreenValue));
    *ptr = value;
    return ptr;
}

CgreenValue make_cgreen_integer_value(intptr_t integer) {
    CgreenValue value = {CGREEN_INTEGER, {}, sizeof(value.value.integer_value)};
    value.value.integer_value = integer;
    return value;
}

CgreenValue make_cgreen_string_value(const char *string) {
    CgreenValue value = {CGREEN_STRING, {}, sizeof(value.value.string_value)};
    value.value.string_value = stringdup(string);
    return value;
}

CgreenValue make_cgreen_pointer_value(void *pointer) {
    CgreenValue value = {CGREEN_POINTER, {}, sizeof(value.value.pointer_value)};
    value.value.pointer_value = pointer;
    return value;
}

CgreenValue make_cgreen_double_value(double d) {
    CgreenValue value = {CGREEN_DOUBLE, {}, sizeof(value.value.double_value)};
    value.value.double_value = d;
    return value;
}

CgreenValue make_cgreen_by_value(void *pointer, size_t size) {
    CgreenValue value = {CGREEN_BYVALUE, {0}, size};
    value.value.pointer_value = pointer;
    return value;
}

void destroy_cgreen_value(CgreenValue value) {
    if (value.type == CGREEN_STRING)
        free((void *)value.value.string_value);
    else if (value.type == CGREEN_BYVALUE)
        free(value.value.pointer_value);
}
