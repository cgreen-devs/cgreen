#ifndef _SEQUENCE_HEADER_
#define _SEQUENCE_HEADER_

#define integer_sequence(...) _integer_sequence(__FILE__, __LINE__, __VA_ARGS__)
#define string_sequence(...) _string_sequence(__FILE__, __LINE__, __VA_ARGS__)
#define address_sequence(...) _address_sequence(__FILE__, __LINE__, __VA_ARGS__)
#define called_never() _count_calls(__FILE__, __LINE__, 0)
#define called_once() _count_calls(__FILE__, __LINE__, 1)
#define expected_call_count(expected) _count_calls(__FILE__, __LINE__, expected)

int _integer_sequence(const char *file, int line, ...);
char *_string_sequence(const char *file, int line, ...);
void *_address_sequence(const char *file, int line, ...);
void _count_calls(const char *file, int line, int expected);
void replay();
void _recorded_integer(const char *file, int line, int expected);
void walk_counters(void(*walker)(const char *, int, int, int, void *), void *memo);
void clear_sequences();

#endif
