#include "utils.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool panic_use_colours = false;

/* TODO these are actually a duplicate of the ones in text_reporter.c */
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"

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

/* For unittesting */
void panic_set_output_buffer(const char *buffer) {
    panic_message_buffer = (char *)buffer;
}

void panic(const char *filename, int line, const char *fmt, ...) {
    va_list args;
    char buffer[1000];

    va_start(args, fmt);
    sprintf(buffer, "%sCGREEN EXCEPTION%s: <%s:%d>",
            panic_use_colours?MAGENTA:"",
            panic_use_colours?RESET:"",
            filename, line);
    vsprintf(&buffer[strlen(buffer)], fmt, args);
    va_end(args);

    if (panic_message_buffer != NULL)
        strcpy(panic_message_buffer, buffer);
    else
        fprintf(stderr, "%s\n", buffer);
}
