#ifndef MOCKS_HEADER
#define MOCKS_HEADER

#include <cgreen/mock_table.h>
#include <cgreen/constraint.h>
#include <cgreen/reporter.h>
#include <cgreen/vector.h>
#include <stdint.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#define expect(f, ...) expect_(get_test_reporter(), #f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define always_expect(f, ...) always_expect_(get_test_reporter(), #f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
#define expect_never(f, ...) expect_never_(get_test_reporter(), #f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)

extern const int UNLIMITED_TIME_TO_LIVE;

#define mock(...) PP_NARG(__VA_ARGS__)(get_test_reporter(), __func__, #__VA_ARGS__, (intptr_t)__VA_ARGS__ +0)
intptr_t mock_(TestReporter *test_reporter, const char *function, const char *parameters, ...);

void expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);
void always_expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);
void expect_never_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);

#define when(parameter, constraint) when_(#parameter, constraint)
Constraint *when_(const char *parameter, Constraint *constraint);

void clear_mocks(void);
void tally_mocks(TestReporter *reporter);

#ifdef __cplusplus
    }
}
#endif

#endif
