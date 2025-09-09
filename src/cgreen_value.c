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
    CgreenValue value;
    value.type = CGREEN_INTEGER;
    value.value_size = sizeof(intptr_t);
    memset(&value.value, 0, sizeof(value.value));
    value.value.integer_value = integer;
    return value;
}

CgreenValue make_cgreen_string_value(const char *string) {
    CgreenValue value = {CGREEN_STRING, {0}, sizeof(const char *)};
    value.value.string_value = stringdup(string);
    return value;
}

CgreenValue make_cgreen_pointer_value(void *pointer) {
    CgreenValue value = {CGREEN_POINTER, {0}, sizeof(intptr_t)};
    value.value.pointer_value = pointer;
    return value;
}

CgreenValue make_cgreen_double_value(double d) {
    CgreenValue value = {CGREEN_DOUBLE, {0}, sizeof(intptr_t)};
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
