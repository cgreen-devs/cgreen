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
    // Setting the environment variable here does not work
    // It needs to be set before the runner forks
    //    setenv("CGREEN_PER_TEST_TIMEOUT", "2", true);
    // so we leave that to the test environment...

    // And fail if there is no environment variable set
    assert_that(getenv("CGREEN_PER_TEST_TIMEOUT"), is_not_equal_to(NULL));
    sleep(3);
    fail_test("This test should have been aborted within CGREEN_PER_TEST_TIMEOUT seconds and not get here. When running this test you need to define CGREEN_PER_TEST_TIMEOUT=2.");
}

#ifdef __cplusplus
Ensure(FailureMessage, increments_exception_count_when_throwing) {
    throw;
}
#endif

