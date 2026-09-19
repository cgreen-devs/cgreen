#include <cgreen/cgreen.h>

Describe(Cgreen);
BeforeEach(Cgreen) {}
AfterEach(Cgreen) {}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    return run_test_suite(suite, create_text_reporter());
}
