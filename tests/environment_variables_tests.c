#define _POSIX_C_SOURCE 200809L
#include <cgreen/cgreen.h>

/* TODO: are these include necessary? */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(EnvironmentVariables);
BeforeEach(EnvironmentVariables) { setenv("TEST", "test", true); }
AfterEach(EnvironmentVariables) {}

Ensure(EnvironmentVariables, set_in_before_each_should_be_set_in_test) {
    assert_that(getenv("TEST"), is_equal_to_string("test"));
}
