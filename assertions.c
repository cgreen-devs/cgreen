#include "assertions.h"
#include "reporter.h"

void _assert_equal(const char *file, int line, intptr_t tried, intptr_t expected) {
    (*get_test_reporter()->assert_true)(
            get_test_reporter(),
            file,
            line,
            (tried == expected),
            "Result [%d] does not match [%d]", tried, expected);
}
