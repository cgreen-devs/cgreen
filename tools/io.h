#ifndef IO_H
#define IO_H

#include <stdio.h>

#ifdef UNITTESTING
#define open_file(x,y) open_file_unittesting(x,y)
#define close_file(x) close_file_unittesting(x)
#define open_process(x,y) open_process_file_unittesting(x,y)
#define close_process(x) close_process_unittesting(x)
#define read_line(x,y,z) read_line_unittesting(x,y,z)
#endif

extern FILE *open_file(const char *filename, const char *mode);
extern int close_file(FILE *file);

extern FILE *open_process(const char *command, const char *mode);
extern int close_process(FILE *file);

extern int read_line(FILE *file, char *buffer, int max_length);

#endif
