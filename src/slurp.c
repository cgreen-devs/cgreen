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
    char *content = (char *)malloc((gulp + 1) * sizeof(char));
    char *block = content;
    for ( ; ; ) {
        printf("Get %d bytes into %p of %p\n", gulp + 1, block, content);
        if (fgets(block, gulp + 1, file) == NULL) {
            printf("Bye bye\n");
            break;
        }
        block += gulp;
        printf("%d byte%s at %p in %p -> ", gulp, gulp == 1 ? "" : "s", block, content);
        content = (char *)realloc(content, (block - content + 1) * sizeof(char));
        printf("%p of %d bytes [%s]\n", content, block - content + 1, content);
    }
    printf("Read %s\n", content);
    return content;
}
