#include <cgreen/cgreen.h>

Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

Ensure(Cgreen, passes_this_test) {
    assert_that(1 == 1);
}

Ensure(Cgreen, fails_this_test) {
    assert_that(0 == 1);
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Cgreen, passes_this_test);
    add_test_with_context(suite, Cgreen, fails_this_test);
    return run_test_suite(suite, create_text_reporter());
}
