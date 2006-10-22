#include "assertions.h"
#include "reporter.h"

void _assert_equal(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried == expected),
            "[%d] should match [%d]", tried, expected);
}

void _assert_not_equal(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried != expected),
            "[%d] should not match [%d]", tried, expected);
}

void _assert_string_equal(const char *file, int line, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            strcmp(tried, expected) == 0,
            "[%s] should match [%s]", tried, expected);
}

void _assert_string_not_equal(const char *file, int line, const char *tried, const char *expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            strcmp(tried, expected) != 0,
            "[%s] should not match [%s]", tried, expected);
}
