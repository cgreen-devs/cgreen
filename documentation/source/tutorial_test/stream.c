#include <stdlib.h>
#include <stdio.h>

char *read_paragraph(int (*read)(void *), void *stream) {
    int buffer_size = 0, length = 0;
    char *buffer = NULL;
    int ch;
    while ((ch = (*read)(stream)) != EOF) {
        if (++length > buffer_size) {
            buffer_size += 100;
            buffer = realloc(buffer, buffer_size + 1);
        }
        if ((buffer[length] = ch) == '\n') {
            break;
        }
    }
    return buffer;
}
