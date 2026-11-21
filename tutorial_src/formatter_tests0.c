#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "stream.h"

Describe(Formatter);
BeforeEach(Formatter) {}
AfterEach(Formatter) {}

int one_character_stream(void *stream) { return 'a'; }

void expect_one_letter_paragraph(void *stream, char *paragraph) {
    assert_that(paragraph, is_equal_to_string("a"));
}

Ensure(Formatter, makes_one_letter_paragraph_from_one_character_input) {
    by_paragraph(
            &one_character_stream,
            NULL,
            &expect_one_letter_paragraph,
            NULL);
}
