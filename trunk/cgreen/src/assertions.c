#include "assertions.h"
#include "reporter.h"
#include <stdlib.h>

void assert_equal_(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried == expected),
            "[%d] should match [%d]", tried, expected);
}

void assert_not_equal_(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried != expected),
            "[%d] should not match [%d]", tried, expected);
}

void assert_string_equal_(const char *file, int line, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            strings_are_equal(tried, expected),
            "[%s] should match [%s]", show_null_as_the_string_null(tried), show_null_as_the_string_null(expected));
}

void assert_string_not_equal_(const char *file, int line, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            ! strings_are_equal(tried, expected),
            "[%s] should not match [%s]", show_null_as_the_string_null(tried), show_null_as_the_string_null(expected));
}

const char *show_null_as_the_string_null(const char *string) {
    return (string == NULL ? "NULL" : string);
}

int strings_are_equal(const char *tried, const char *expected) {
    if ((tried == NULL) || (expected == NULL)) {
        return (tried == expected);
    } else {
        return (strcmp(tried, expected) == 0);
    }
}
