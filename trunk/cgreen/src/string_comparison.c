#include <cgreen/string_comparison.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

bool strings_are_equal(const char* actual, const char* expected) {
    /* TODO: if expected is null, warn user to use appropriate non-string assert instead */
    if ((actual == NULL) && (expected == NULL)) {
        return true;
    }

    if ((actual == NULL) || (expected == NULL)) {
        return false;
    }

    return (strcmp(actual, expected) == 0);
}

bool string_contains(const char* actual, const char* expected) {
    /* TODO: if expected is null, warn user */
    if ((actual == NULL) || (expected == NULL)) {
        return false;
    }

    return (strstr(actual, expected) != NULL);
}


#ifdef __cplusplus
}
#endif
