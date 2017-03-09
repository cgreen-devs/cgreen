#include <cgreen/cgreen.h>
#include <cgreen/internal/circular_buffer.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(CircularBuffer);

static CircularBuffer* instance;
static const int length=100;

BeforeEach(CircularBuffer) {
    instance = create_circular_buffer(length);
}
AfterEach(CircularBuffer) {
    destroy_circular_buffer(instance);
}

Ensure(CircularBuffer, created_and_correctly_initialized) {
    assert_that(instance, is_non_null);
    assert_that(instance->buffer, is_non_null);
    assert_that(instance->buffer_end >= (instance->buffer + length));
    assert_that(instance->head, is_equal_to( instance->buffer ));
    assert_that(instance->tail, is_equal_to( instance->head ));
}

Ensure(CircularBuffer, accepts_data) {
    for(int i=0; i<100; i++)
        assert_that(write_to_circular_buffer(instance, i), is_equal_to(1));
    assert_that(write_to_circular_buffer(instance, 100), is_equal_to(0)); // full
}


TestSuite *circular_buffer_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, CircularBuffer, created_and_correctly_initialized);
    add_test_with_context(suite, CircularBuffer, accepts_data);

    return suite;
}

/* vim: set ts=4 sw=4 et cindent: */
