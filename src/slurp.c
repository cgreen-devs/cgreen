#include <cgreen/slurp.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

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
    char *content = (char *)malloc(0);
    int sblock = (gulp + 1) * sizeof(char);
    char *block = (char *)malloc(sblock);
    int len = 0;
    int add = 0;
    char *p;
    for ( ; ; ) {
        if (fgets(block, sblock, file) == NULL) {
            break;
        }
	
        len  = strlen(block);
	add += len;

        p = (char *)realloc(content, add + 1);
	if (p == NULL) {
		exit(1);
	}
        content = p;

	strncat(content, block, len);
    }
    content[add + 1] = '\0';
    free(block);
    return content;
}

