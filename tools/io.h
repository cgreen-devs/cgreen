#ifndef IO_H
#define IO_H

#include <stdio.h>

#ifdef UNITTESTING
#define open_file open_file_unittesting
#define close_file close_file_unittesting
#define open_process open_process_file_unittesting
#define close_process close_process_unittesting
#define read_line read_line_unittesting
#endif

extern FILE *open_file(const char *filename, const char *mode);
extern int close_file(FILE *file);

extern FILE *open_process(const char *command, const char *mode);
extern int close_process(FILE *file);

extern int read_line(FILE *file, char *buffer, int max_length);

#endif
