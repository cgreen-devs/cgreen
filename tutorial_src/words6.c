#include <string.h>

int split_words(char *sentence) {
  int i, count = 1, length = strlen(sentence);
  for (i = 0; i < length; i++) {
    if (sentence[i] == ' ') {
      sentence[i] = '\0';
      count++;
    }
  }
  return count;
}

void words(const char *sentence, void (*callback)(const char *, void *), void *memo) {
  (*callback)(sentence, memo);
}
