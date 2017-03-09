#include <cgreen/cgreen.h>
#include <cgreen/internal/circular_buffer.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(CircularBuffer);

BeforeEach(CircularBuffer) {}
AfterEach(CircularBuffer) {}

Ensure(CircularBuffer, can_be_created) {
    int length=100;
    assert_that(create_circular_buffer(length), is_non_null);
}


TestSuite *circular_buffer_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, CircularBuffer, can_be_created);

    return suite;
}

