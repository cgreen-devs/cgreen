int split_words(char *sentence);
void words(const char *sentence, void (*walker)(const char *, void *), void *);