#include <cgreen/cgreen.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(IgnoreMessage);
BeforeEach(IgnoreMessage) {}
AfterEach(IgnoreMessage) {}

Ensure(IgnoreMessage, should_not_count_empty_tests_as_ignored) {
}

Ensure(IgnoreMessage, should_not_count_passing_tests_as_ignored) {
    assert_that(true);
}

Ensure(IgnoreMessage, should_not_count_failing_tests_as_ignored) {
    assert_that(false);
}

Ensure(IgnoreMessage, should_not_count_exceptions_as_ignored) {
#ifdef HAVE_SYS_RESOURCE_H
    struct rlimit core_limit;
    core_limit.rlim_cur = 1U;
    core_limit.rlim_max = 1U;
    setrlimit(RLIMIT_CORE, &core_limit);
#endif
    raise(SIGSEGV);
}

xEnsure(IgnoreMessage, should_present_this_as_ignored) {
    fail_test("Should not be run since it's inside an ignored test");
}
