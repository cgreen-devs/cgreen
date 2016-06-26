#include <cgreen/cgreen.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(IgnoreMessage);
BeforeEach(IgnoreMessage) {}
AfterEach(IgnoreMessage) {}

Ensure(IgnoreMessage, should_not_count_empty_tests_as_ignored) {
}

Ensure(IgnoreMessage, should_not_count_passing_tests_as_ignored) {
}

Ensure(IgnoreMessage, should_not_count_failing_tests_as_ignored) {
}

Ensure(IgnoreMessage, should_not_count_exceptions_as_ignored) {
    int segv = *((int*)1);
    (void)segv;                 /* UNUSED */
}

xEnsure(IgnoreMessage, should_present_this_as_ignored) {
    fail_test("Should not be run since it's inside an ignored test");
}


