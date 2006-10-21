#ifndef _MOCKS_HEADER_
#define _MOCKS_HEADER_

#include <inttypes.h>
#include "reporter.h"

#define ANYTHING "__anything__"

#define compare_integer(i) _compare_integer(__FILE__, __LINE__, #i, i)
#define compare_string(s) _compare_string(__FILE__, __LINE__, #s, s)
#define compare_address(a) _compare_address(__FILE__, __LINE__, #a, a)
#define expect(f, ...) _expect(#f, __FILE__, __LINE__); _mask(#__VA_ARGS__); f(__VA_ARGS__); _play()
#define will_return(f, r) _stub(#f, __FILE__, __LINE__); _will_return(#f, (intptr_t)r); _play()
#define mock(f, r, ...) _mock(#f, __FILE__, __LINE__); _mask(#__VA_ARGS__); f(__VA_ARGS__); _will_return(#f, (intptr_t)r); _play()
#define result() _result(__func__)
#define always _always();

void _compare_integer(const char *file, int line, const char *parameter, int integer);
void _compare_string(const char *file, int line, const char *parameter, int string);
void _compare_address(const char *file, int line, const char *parameter, int address);
intptr_t _result(const char *function);
void _expect(const char *function, const char *file, int line);
void _stub(const char *function, const char *file, int line);
void _mock(const char *function, const char *file, int line);
void _play();
void _will_return(const char *function, intptr_t result);
void _mask(const char *parameters);
void _always();
void clear_mocks();
void tally_mocks(TestReporter *reporter);

#endif
