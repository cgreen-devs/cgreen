#include <cgreen/cgreen.h>

TestSuite *our_tests();
TestSuite *person_tests();

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_suite(suite, our_tests());
    add_suite(suite, person_tests());
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
