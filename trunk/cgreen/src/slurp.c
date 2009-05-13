#include <cgreen/slurp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	char *content = (char *)malloc(1);
	int sblock = (gulp + 1) * sizeof(char);
	char *block = (char *)malloc(sblock);
	int len, add = 0;

	*content = '\0';
	for ( ; ; ) {
		if (fgets(block, sblock, file) == NULL) {
			break;
		}

		len  = strlen(block);
		add += len;

		content = (char *)realloc(content, add + 1);
		if (content == NULL) {
			exit(1);
		}

		strncat(content, block, len);
	}
	free(block);
	return content;
}

/* vim: set ts=4 sw=4 et cindent: */
