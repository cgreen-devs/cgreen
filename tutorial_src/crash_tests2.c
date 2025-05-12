#include <cgreen/cgreen.h>
#include <stdlib.h>

Describe(CrashExample);
BeforeEach(CrashExample) {}
AfterEach(CrashExample) {}

Ensure(CrashExample, seg_faults_for_null_dereference) {
    int *p = NULL;
    (*p)++;
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, CrashExample, seg_faults_for_null_dereference);
    return run_single_test(suite, "seg_faults_for_null_dereference", create_text_reporter());
}
