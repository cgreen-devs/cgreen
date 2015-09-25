#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *read_paragraph(int (*read)(void *), void *stream) {
    int buffer_size = 0, length = 0;
    char *buffer = NULL;
    int ch;
    while ((ch = (*read)(stream)) != EOF) {
        if (++length > buffer_size) {
            buffer_size += 100;
            buffer = realloc(buffer, buffer_size + 1);
        }
        if ((buffer[length - 1] = ch) == '\n') {
            buffer[--length] = '\0';
            break;
        }
        buffer[length] = '\0';
    }
    return buffer;
}

void by_paragraph(int (*read)(void *), void *in, void (*write)(void *, char *), void *out) {
    while (1) {
        char *line = read_paragraph(read, in);
        if ((line == NULL) || (strlen(line) == 0)) {
            return;
        }
        (*write)(out, line);
        free(line);
    }
}
