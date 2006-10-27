#include "cgreen/cgreen.h"
#include <stdio.h>

char *read_paragraph(int (*read)(void *), void *stream);

static int stub_stream(void *stream) {
    return (int)stubbed_result();
}

static void reading_lines_from_empty_stream_gives_null() {
    always_return(stub_stream, EOF);
    assert_equal(read_paragraph(&stub_stream, NULL), NULL);
}

static TestSuite *stream_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, reading_lines_from_empty_stream_gives_null);
    return suite;
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
	add_suite(suite, stream_tests());
    return run_test_suite(suite, create_text_reporter());
}
