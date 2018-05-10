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
