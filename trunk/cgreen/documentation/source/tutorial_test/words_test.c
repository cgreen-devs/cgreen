#include "cgreen/cgreen.h"
#include "words.h"
#include <string.h>
#include <stdlib.h>

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

void mocked_callback(char *word, void *memo) {
    checked_string(word);
    checked_integer(memo);
}

void single_word_sentence_invokes_callback_once() {
	expect(mocked_callback, "Word", NULL);
	words("Word", &mocked_callback, NULL);
}

void phrase_invokes_callback_for_each_word() {
	expect(mocked_callback, "Birds", NULL);
	expect(mocked_callback, "of", NULL);
	expect(mocked_callback, "a", NULL);
	expect(mocked_callback, "feather", NULL);
	words("Birds of a feather", &mocked_callback, NULL);
}

TestSuite *words_tests() {
    TestSuite *suite = create_test_suite();
	add_unit_test(suite, word_count_returned_from_split);
	add_unit_test(suite, spaces_should_be_converted_to_zeroes);
	add_unit_test(suite, single_word_sentence_invokes_callback_once);
	add_unit_test(suite, phrase_invokes_callback_for_each_word);
    return suite;
}
