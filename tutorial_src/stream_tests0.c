#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

char *read_paragraph(int (*read)(void *), void *stream);

static int empty_stream(void *stream) {
    return EOF;
}

Describe(ParagraphReader);
BeforeEach(ParagraphReader) {}
AfterEach(ParagraphReader) {}

Ensure(ParagraphReader, gives_null_when_reading_empty_stream) {
    assert_that(read_paragraph(&empty_stream, NULL), is_null);
}
