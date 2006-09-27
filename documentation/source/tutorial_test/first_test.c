#include "cgreen/cgreen.h"

void this_test_should_pass() {
	assert_true(1, "This should be true");
}

void this_test_should_fail() {
	assert_true(0, "This should be false");
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
	add_unit_test(suite, this_test_should_pass);
	add_unit_test(suite, this_test_should_fail);
    return run_test_suite(suite, create_text_reporter());
}
