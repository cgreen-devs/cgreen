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

void words(const char *sentence, void (*walker)(const char *, void *), void *memo) {
    char *words = strdup(sentence);
    int word_count = split_words(words);
    char *word = words;
    while (word_count-- > 0) {
        (*walker)(word, memo);
        word = word + strlen(word) + 1;
    }
    free(words);
}
