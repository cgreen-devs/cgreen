#include "cgreen/cgreen.h"
#include "words.h"
#include <string.h>
#include <stdlib.h>

void word_count_returned_from_split() {
	char *sentence = strdup("Birds of a feather");
	int word_count = split_words(sentence);
	assert_equal(word_count, 4, "Word count of [%s] was [%d]", sentence, word_count);
	free(sentence);
}

void spaces_should_be_converted_to_zeroes() {
	char *sentence = strdup("Birds of a feather");
	split_words(sentence);
	int comparison = memcmp("Birds\0of\0a\0feather", sentence, strlen(sentence));
	assert_equal(comparison, 0, "Compares as %d", comparison);
	free(sentence);	
}

void one_word(const char *word, void *i) {
	assert_string_equal(word, "Word", NULL);
	*(int *)i = 1;
}

void single_word_sentence_invokes_callback_once() {
	int i = 0;
	words("Word", &one_word, &i);
	assert_true(i, NULL);
}

void four_words(const char *word, void *memo) {
	assert_string_equal(word, string_sequence("Birds", "of", "a", "feather"));
	expected_call_count(4);
}

void phrase_invokes_callback_for_each_word() {
	words("Birds of a feather", &four_words, NULL);
}

TestSuite *words_tests() {
    TestSuite *suite = create_test_suite();
	add_unit_test(suite, word_count_returned_from_split);
	add_unit_test(suite, spaces_should_be_converted_to_zeroes);
	add_unit_test(suite, single_word_sentence_invokes_callback_once);
	add_unit_test(suite, phrase_invokes_callback_for_each_word);
    return suite;
}
