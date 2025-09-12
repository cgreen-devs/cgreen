#include <cgreen/cgreen.h>

#include "words.h"
#include <string.h>

Describe(Words);
BeforeEach(Words) {}
AfterEach(Words) {}

Ensure(Words, returns_word_count) {
    char *sentence = strdup("Birds of a feather");
    int word_count = split_words(sentence);
    assert_that(word_count, is_equal_to(4));
    free(sentence);
}

TestSuite *words_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Words, returns_word_count);
    return suite;
}
