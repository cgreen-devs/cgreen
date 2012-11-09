#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

char *read_paragraph(int (*read)(void *), void *stream);

static int stub_stream(void *stream) {
    return (int)mock(stream);
}

Ensure(reading_lines_from_empty_stream_gives_null) {
    always_expect(stub_stream, will_return(EOF));
    assert_that(read_paragraph(&stub_stream, NULL), is_null);
}

TestSuite *stream_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, reading_lines_from_empty_stream_gives_null);
    return suite;
}

int main(int argc, char **argv) {
    TestSuite *suite = stream_tests();
    if (argc > 1) {
        return run_single_test(suite, argv[1], create_text_reporter());
    }
    return run_test_suite(suite, create_text_reporter());
}
