#include <cgreen/cgreen.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(FailureMessage);
BeforeEach(FailureMessage) {}
AfterEach(FailureMessage) {}

Ensure(FailureMessage, for_time_out_in_only_one_second) {
    die_in(1);
    sleep(10);
}

Ensure(FailureMessage, for_CGREEN_PER_TEST_TIMEOUT) {
    setenv("CGREEN_PER_TEST_TIMEOUT", "2", false);
    sleep(10);
}
