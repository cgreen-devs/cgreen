#include "test_item.h"

#include <stdlib.h>
#include <string.h>

#include <cgreen/internal/unit_implementation.h>

#include <stdio.h>


static const char *skip_to_separator(const char *start) {
    return strstr(start, CGREEN_SEPARATOR);
}

static const char *skip_over_separator(const char *start) {
    return strstr(start, CGREEN_SEPARATOR)+strlen(CGREEN_SEPARATOR);
}

static const char *skip_cgreen_spec_marker(const char *string) {
    return &string[strlen(CGREEN_SPEC_PREFIX CGREEN_SEPARATOR)];
}

static const char *string_copy_of(const char *start, const char *end) {
    char *string = strdup(start);
    string[end-start] = '\0';
    return string;
}

static const char *context_name_from(const char *specification_name) {
    const char *start = skip_cgreen_spec_marker(specification_name);
    const char *end = skip_to_separator(start);
    return string_copy_of(start, end);
}

static const char *test_name_from(const char *specification_name) {
    const char *start = skip_cgreen_spec_marker(specification_name);
    start = skip_over_separator(start);
    const char *end = skip_to_separator(start);
    return string_copy_of(start, end);
}

TestItem *create_test_item_from(const char *specification_name) {
    char name[strlen(specification_name)+1];
    TestItem *test_item = (TestItem *)malloc(sizeof(TestItem));

    strcpy(name, &specification_name[strlen(CGREEN_SPEC_PREFIX CGREEN_SEPARATOR)]);
    test_item->specification_name = strdup(specification_name);
    test_item->context_name = context_name_from(specification_name);
    test_item->test_name = test_name_from(specification_name);

    return test_item;
}
