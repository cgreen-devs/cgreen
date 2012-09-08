#include <cgreen/cgreen.h>
#include <stdlib.h>

Ensure(will_seg_fault) {
    int *p = NULL;
    (*p)++;
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test(suite, will_seg_fault);
    run_test_suite(suite, create_text_reporter());
}
