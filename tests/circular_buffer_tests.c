#include <cgreen/cgreen.h>
#include <cgreen/constraint.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(CircularBuffer);
BeforeEach(CircularBuffer) {}
AfterEach(CircularBuffer) {}

TestSuite *circular_buffer_tests() {
    TestSuite *suite = create_test_suite();

//    add_test_with_context(suite, Breadcrumb, can_destroy_empty_breadcrumb);

    return suite;
}

