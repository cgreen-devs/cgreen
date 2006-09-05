#ifndef _SEQUENCE_HEADER_
#define _SEQUENCE_HEADER_

int _integer_sequence(const char *file, int line, ...);
char *_string_sequence(const char *file, int line, ...);
void *_address_sequence(const char *file, int line, ...);
void _count_calls(const char *file, int line, int expected);
void walk_counters(void(*walker)(const char *, int, int, int, void *), void *memo);
void clear_sequences();

#endif
