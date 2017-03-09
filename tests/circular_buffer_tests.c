#include <cgreen/cgreen.h>
#include <cgreen/internal/circular_buffer.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(CircularBuffer);

BeforeEach(CircularBuffer) {}
AfterEach(CircularBuffer) {}

Ensure(CircularBuffer, created_and_correctly_initialized) {
    int length=100;
    CircularBuffer* instance = create_circular_buffer(length);
    assert_that(instance, is_non_null);
    assert_that(instance->buffer, is_non_null);
    assert_that(instance->buffer_end, is_equal_to( (instance->buffer + length) ));
    assert_that(instance->head, is_equal_to( instance->buffer ));
    assert_that(instance->tail, is_equal_to( instance->head ));
}


TestSuite *circular_buffer_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, CircularBuffer, created_and_correctly_initialized);

    return suite;
}

/* vim: set ts=4 sw=4 et cindent: */
