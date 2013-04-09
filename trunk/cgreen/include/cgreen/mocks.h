#ifndef MOCKS_HEADER
#define MOCKS_HEADER

#include <cgreen/internal/mock_table.h>
#include <cgreen/internal/function_macro.h>
#include <cgreen/constraint.h>
#include <cgreen/reporter.h>
#include <cgreen/vector.h>
#include <stdint.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#define expect(f, ...) expect_(get_test_reporter(), #f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
void expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);

#define always_expect(f, ...) always_expect_(get_test_reporter(), #f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
void always_expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);

#define never_expect(f, ...) never_expect_(get_test_reporter(), #f, __FILE__, __LINE__, (Constraint *)__VA_ARGS__ +0, (Constraint *)0)
void never_expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);

#ifdef _MSC_VER
// another workaround for fundamental variadic macro deficiencies in Visual C++ 2012
#define mock(...) PP_NARG(__VA_ARGS__)(get_test_reporter(), __func__, __FILE__, __LINE__, #__VA_ARGS__ "", __VA_ARGS__)
#else
#define mock(...) PP_NARG(__VA_ARGS__)(get_test_reporter(), __func__, __FILE__, __LINE__, #__VA_ARGS__ "", __VA_ARGS__ +0)
#endif
intptr_t mock_(TestReporter *test_reporter, const char *function, const char *mock_file, int mock_line, const char *parameters, ...);

#define when(parameter, constraint) when_(#parameter, constraint)
Constraint *when_(const char *parameter, Constraint *constraint);

void clear_mocks(void);
void tally_mocks(TestReporter *reporter);

typedef enum { strict_mocks = 0, loose_mocks = 1, learning_mocks = 2 } CgreenMockMode;

void cgreen_mocks_are(CgreenMockMode);

extern const int UNLIMITED_TIME_TO_LIVE;

#ifdef __cplusplus
    }
}
#endif

#endif
