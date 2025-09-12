#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "words.h"
#include <string.h>

Describe(Words);
BeforeEach(Words) {}
AfterEach(Words) {}

TestSuite *words_tests() {
    TestSuite *suite = create_test_suite();
    return suite;
}
