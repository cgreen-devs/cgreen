#include "io.h"

/* Adapter to C I/O functions */

#include <string.h>


FILE *open_file(const char *filename, const char *mode) {
    return fopen(filename, mode);
}

int close_file(FILE *file) {
    return fclose(file);
}

FILE *open_process(const char *command, const char *mode) {
    return popen(command, mode);
}

int close_process(FILE *process) {
    return pclose(process);
}


/*
  read_line()   - read characters from a FILE
                - behaves like fgets() except will return
                - number of characters read and
                - EOF if at end of file
*/
int read_line(FILE *file, char *buffer, int max_length) {
    char *result = fgets(buffer, max_length, file);
    return result==NULL? EOF : (signed)strlen(result);
}
