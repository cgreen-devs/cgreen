#include <string.h>

int split_words(char *sentence) {
  int i, count = 1;
  for (i = 0; i < strlen(sentence); i++) {
    if (sentence[i] == ' ') {
      sentence[i] = '\0';
      count++;
    }
  }
  return count;
}
