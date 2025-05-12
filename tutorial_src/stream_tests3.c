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

Ensure(ParagraphReader, gives_one_character_line_for_one_character_stream) {
    expect(stream_stub, will_return('a'));
    expect(stream_stub, will_return(EOF));
    char *line = read_paragraph(&stream_stub, NULL);
    assert_that(line, is_equal_to_string("a"));
    free(line);
}

Ensure(ParagraphReader, gives_one_word_line_for_one_word_stream) {
    expect(stream_stub, will_return('t'));
    expect(stream_stub, will_return('h'));
    expect(stream_stub, will_return('e'));
    always_expect(stream_stub, will_return(EOF));
    assert_that(read_paragraph(&stream_stub, NULL), is_equal_to_string("the"));
}
