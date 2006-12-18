#include "cgreen/cgreen.h"
#include "words.h"
#include <string.h>

void word_count_returned_from_split() {
    char *sentence = strdup("Birds of a feather");
    int word_count = split_words(sentence);
    assert_equal(word_count, 4);
    free(sentence);
}

void spaces_should_be_converted_to_zeroes() {
    char *sentence = strdup("Birds of a feather");
    split_words(sentence);
    int comparison = memcmp("Birds\0of\0a\0feather", sentence, strlen(sentence));
    assert_equal(comparison, 0);
    free(sentence); 
}

void mocked_callback(const char *word, void *memo) {
    mock(word, memo);
}

void single_word_sentence_invokes_callback_once() {
    expect(mocked_callback, want_string(word, "Word"), want(memo, NULL));
    words("Word", &mocked_callback, NULL);
}

void phrase_invokes_callback_for_each_word() {
    expect(mocked_callback, want_string(word, "Birds"));
    expect(mocked_callback, want_string(word, "of"));
    expect(mocked_callback, want_string(word, "a"));
    expect(mocked_callback, want_string(word, "feather"));
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