#include <cgreen/cgreen.h>

Ensure(failing_test_is_listed_by_libxml_reporter) {
    assert_that(false);
}

Ensure(passing_test_is_listed_by_libxml_reporter) {
    assert_that(true);
}
