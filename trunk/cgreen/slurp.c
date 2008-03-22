#include "slurp.h"
#include <stdlib.h>
#include <stdio.h>

static char *read_all(FILE *file, int gulp);

char *slurp(const char *file_name, int gulp) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return NULL;
    }
    char *content = read_all(file, gulp);
    fclose(file);
    return content;
}

static char *read_all(FILE *file, int gulp) {
    char *content = (char *)malloc(gulp * sizeof(char));
    char *block = content;
    for ( ; ; ) {
        if (fgets(block, gulp + 1, file) == NULL) {
            break;
        }
        block += gulp;
        content = (char *)realloc(content, (block - content) * sizeof(char));
    }
    return content;
}
