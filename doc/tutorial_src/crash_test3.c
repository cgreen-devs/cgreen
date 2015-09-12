#include <cgreen/cgreen.h>
#include <stdlib.h>

Describe(CrashExample);
BeforeEach(CrashExample) {}
AfterEach(CrashExample) {}

Ensure(CrashExample, seg_faults_for_null_dereference) {
    int *p = NULL;
    (*p)++;
}

Ensure(CrashExample, will_loop_forever) {
    die_in(1);
    while(0 == 0) { }
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, CrashExample, seg_faults_for_null_dereference);
    add_test_with_context(suite, CrashExample, will_loop_forever);
    return run_test_suite(suite, create_text_reporter());
}
