#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "stream.h"

Describe(Formatter);
BeforeEach(Formatter) {}
AfterEach(Formatter) {}

static int reader(void *stream) {
    return (int)mock(stream);
}

static void writer(void *stream, char *paragraph) {
    mock(stream, paragraph);
}

Ensure(Formatter, makes_one_letter_paragraph_from_one_character_input) {
    expect(reader, will_return('a'));
    always_expect(reader, will_return(EOF));
    expect(writer, when(paragraph, is_equal_to_string("a")));
    by_paragraph(&reader, NULL, &writer, NULL);
}

Ensure(Formatter, makes_one_paragraph_if_no_line_endings) {
    expect(reader, will_return('a'));
    expect(reader, will_return(' '));
    expect(reader, will_return('b'));
    expect(reader, will_return(' '));
    expect(reader, will_return('c'));
    always_expect(reader, will_return(EOF));
    expect(writer, when(paragraph, is_equal_to_string("a b c")));
    by_paragraph(&reader, NULL, &writer, NULL);
}

Ensure(Formatter, generates_separate_paragraphs_for_line_endings) {
    expect(reader, will_return('a'));
    expect(reader, will_return('\n'));
    expect(reader, will_return('b'));
    expect(reader, will_return('\n'));
    expect(reader, will_return('c'));
    always_expect(reader, will_return(EOF));
    expect(writer, when(paragraph, is_equal_to_string("a")));
    expect(writer, when(paragraph, is_equal_to_string("b")));
    expect(writer, when(paragraph, is_equal_to_string("c")));
    by_paragraph(&reader, NULL, &writer, NULL);
}

Ensure(Formatter, pairs_the_functions_with_the_resources) {
    expect(reader, when(stream, is_equal_to(1)), will_return('a'));
    always_expect(reader, when(stream, is_equal_to(1)), will_return(EOF));
    expect(writer, when(stream, is_equal_to(2)));
    by_paragraph(&reader, (void *)1, &writer, (void *)2);
}

