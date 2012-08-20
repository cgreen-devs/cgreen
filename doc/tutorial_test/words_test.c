#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "words.h"
#include <string.h>

Ensure(word_count_returned_from_split) {
    char *sentence = strdup("Birds of a feather");
    int word_count = split_words(sentence);
    assert_equal(word_count, 4);
    free(sentence);
}

Ensure(spaces_should_be_converted_to_zeroes) {
    char *sentence = strdup("Birds of a feather");
    split_words(sentence);
    int comparison = memcmp("Birds\0of\0a\0feather", sentence, strlen(sentence));
    assert_equal(comparison, 0);
    free(sentence); 
}

void mocked_callback(const char *word, void *memo) {
    mock(word, memo);
}

Ensure(single_word_sentence_invokes_callback_once) {
  expect(mocked_callback, when(word, is_equal_to_string("Word")), when(memo, is_equal_to(NULL)));
  words("Word", &mocked_callback, NULL);
}

Ensure(phrase_invokes_callback_for_each_word) {
    expect(mocked_callback, when(word, is_equal_to_string("Birds")));
    expect(mocked_callback, when(word, is_equal_to_string("of")));
    expect(mocked_callback, when(word, is_equal_to_string("a")));
    expect(mocked_callback, when(word, is_equal_to_string("feather")));
    words("Birds of a feather", &mocked_callback, NULL);
}

TestSuite *words_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, word_count_returned_from_split);
    add_test(suite, spaces_should_be_converted_to_zeroes);
    add_test(suite, single_word_sentence_invokes_callback_once);
    add_test(suite, phrase_invokes_callback_for_each_word);
    return suite;
}
