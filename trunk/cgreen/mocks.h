#ifndef _MOCKS_HEADER_
#define _MOCKS_HEADER_

#include <inttypes.h>
#include "reporter.h"

#define ANYTHING "__anything__"

#define checked_integer(i) _checked_integer(__FILE__, __LINE__, #i, i)
#define checked_string(s) _checked_string(__FILE__, __LINE__, #s, s)
#define expect(f, ...) _expect(#f, __FILE__, __LINE__); _mask(#__VA_ARGS__); f(__VA_ARGS__); _play()
#define always_expect(f, ...) _always(); _expect(#f, __FILE__, __LINE__); _mask(#__VA_ARGS__); f(__VA_ARGS__); _play()
#define will_return(f, r) _will_return(#f, (intptr_t)r); _play()
#define always_return(f, r) _always(); _will_return(#f, (intptr_t)r); _play()
#define mock(f, r, ...) _mock(#f, __FILE__, __LINE__); _mask(#__VA_ARGS__); f(__VA_ARGS__); _will_return(#f, (intptr_t)r); _play()
#define stubbed_result() _stubbed_result(__func__, __FILE__, __LINE__)

void _checked_integer(const char *check_file, int check_line, const char *parameter, int integer);
void _checked_string(const char *check_file, int check_line, const char *parameter, const char *string);
intptr_t _stubbed_result(const char *function, const char *file, int line);
void _expect(const char *function, const char *test_file, int test_line);
void _mock(const char *function, const char *test_file, int test_line);
void _play();
void _will_return(const char *function, intptr_t result);
void _mask(const char *parameters);
void _always();
void clear_mocks();
void tally_mocks(TestReporter *reporter);

#endif
