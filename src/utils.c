#include "utils.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../gitrevision.h"

char *cgreen_library_version = VERSION;
char *cgreen_library_revision = GITREVISION;

char *string_dup(const char *string) {
    char *dup = (char *)malloc(strlen(string)+1);
    if (dup)
        strcpy(dup, string);
    return dup;
}


static char *panic_message_buffer = NULL;

void panic_set_output_buffer(const char *buffer) {
    panic_message_buffer = (char *)buffer;
}

void panic(const char *filename, int line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (panic_message_buffer == NULL) {
        fprintf(stderr, "CGREEN EXCEPTION: <%s:%d>", filename, line);
        vfprintf(stderr, fmt, args);
    } else {
        sprintf(panic_message_buffer, "CGREEN EXCEPTION: <%s:%d>", filename, line);
        vsprintf(&panic_message_buffer[strlen(panic_message_buffer)], fmt, args);
    }
    va_end(args);
}
