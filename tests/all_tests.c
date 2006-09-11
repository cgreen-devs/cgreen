#include "../cgreen.h"

TestSuite *messaging_tests();

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, messaging_tests());
    return run_test_suite(suite, create_text_reporter());
}
