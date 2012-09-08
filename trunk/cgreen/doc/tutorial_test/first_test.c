#include <cgreen/cgreen.h>

Ensure(this_test_should_pass) {
    assert_true(1 == 1);
}

Ensure(this_test_should_fail) {
    assert_true(0 == 1);
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test(suite, this_test_should_pass);
    add_test(suite, this_test_should_fail);
    return run_test_suite(suite, create_text_reporter());
}
