#include <stdio.h>

extern FILE *open_file(const char *filename, const char *mode);
extern FILE *open_process(const char *command, const char *mode);
extern int read_line(FILE *file, char *buffer, int max_length);
