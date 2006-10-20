#ifndef _MOCKS_HEADER_
#define _MOCKS_HEADER_

#define recorded_integer(i) _recorded_integer(__FILE__, __LINE__, i)
#define recorded_string(s) _recorded_string(__FILE__, __LINE__, s)
#define recorded_address(a) _recorded_address(__FILE__, __LINE__, a)

void _recorded_integer(const char *file, int line, int integer);
void _recorded_string(const char *file, int line, char *string);
void _recorded_address(const char *file, int line, void *address);

#endif
