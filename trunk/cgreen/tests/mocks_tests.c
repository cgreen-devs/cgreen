#include "../cgreen.h"
#include "../mocks.h"
#include <stdlib.h>

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    return suite;
}
