#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

char *read_paragraph(int (*read)(void *), void *stream);

static int stream_stub(void *stream) {
    return (int)mock(stream);
}

Describe(ParagraphReader);
BeforeEach(ParagraphReader) {}
AfterEach(ParagraphReader) {}

Ensure(ParagraphReader, gives_null_when_reading_empty_stream) {
    always_expect(stream_stub, will_return(EOF));                                 // <1>
    assert_that(read_paragraph(&stream_stub, NULL), is_null);
}
