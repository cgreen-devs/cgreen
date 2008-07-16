#ifndef MOCKS_HEADER
#define MOCKS_HEADER

#include <inttypes.h>
#include <stdarg.h>
#include <cgreen/reporter.h>
#include <cgreen/constraint.h>

#define mock(...) mock_(__func__, #__VA_ARGS__, (intptr_t)__VA_ARGS__ +0)
#define expect(f, ...) expect_(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define always_expect(f, ...) always_expect_(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define expect_never(f) expect_never_(#f, __FILE__, __LINE__)
#define will_return(f, r) will_return_(#f, (intptr_t)r)
#define always_return(f, r) always_return_(#f, (intptr_t)r)
#define will_respond(f, r, ...) will_return_(#f, (intptr_t)r); expect_(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define always_respond(f, r, ...) always_return_(#f, (intptr_t)r); always_expect_(#f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)

intptr_t _mock(const char *function, const char *parameters, ...);
void expect_(const char *function, const char *test_file, int test_line, ...);
void always_expect_(const char *function, const char *test_file, int test_line, ...);
void expect_never_(const char *function, const char *test_file, int test_line);
void will_return_(const char *function, intptr_t result);
void always_return_(const char *function, intptr_t result);
void clear_mocks();
void tally_mocks(TestReporter *reporter);

#endif
