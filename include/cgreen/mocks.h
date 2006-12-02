#ifndef _MOCKS_HEADER_
#define _MOCKS_HEADER_

#include <inttypes.h>
#include <stdarg.h>
#include "reporter.h"
#include "constraint.h"

#define checked_integer(i) _checked_integer(__FILE__, __LINE__, __func__, #i, (intptr_t)i)
#define checked_string(s) _checked_string(__FILE__, __LINE__, __func__, #s, s)
#define stubbed_result() _stubbed_result(__func__, __FILE__, __LINE__)
#define expect(f, ...) _expect(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define always_expect(f, ...) _always_expect(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define expect_never(f) _expect_never(#f, __FILE__, __LINE__)
#define will_return(f, r) _will_return(#f, (intptr_t)r)
#define always_return(f, r) _always_return(#f, (intptr_t)r)
#define mock(f, r, ...) _will_return(#f, (intptr_t)r); _expect(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define always_mock(f, r, ...) _always_return(#f, (intptr_t)r); _always_expect(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)

void _checked_integer(const char *check_file, int check_line, const char *function, const char *parameter, intptr_t integer);
void _checked_string(const char *check_file, int check_line, const char *function, const char *parameter, const char *string);
intptr_t _stubbed_result(const char *function, const char *file, int line);
void _expect(const char *function, const char *test_file, int test_line, ...);
void _expect_always(const char *function, const char *test_file, int test_line, ...);
void _expect_never(const char *function, const char *test_file, int test_line);
void _will_return(const char *function, intptr_t result);
void _always_return(const char *function, intptr_t result);
void clear_mocks();
void tally_mocks(TestReporter *reporter);

#endif
