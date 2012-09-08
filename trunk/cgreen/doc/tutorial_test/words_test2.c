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

TestSuite *words_tests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, word_count_returned_from_split);
  add_test(suite, spaces_should_be_converted_to_zeroes);
  return suite;
}
