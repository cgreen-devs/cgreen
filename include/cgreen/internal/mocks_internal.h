#ifndef MOCKS_INTERNAL_H
#define MOCKS_INTERNAL_H

#include <cgreen/internal/mock_table.h>
#include <cgreen/internal/function_macro.h>
#include <cgreen/constraint.h>
#include <cgreen/reporter.h>
#include <cgreen/vector.h>
#include <stdint.h>


#ifdef __GNUC__
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

extern void expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line, ...);
extern void always_expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line,
                           ...);
extern void never_expect_(TestReporter *test_reporter, const char *function, const char *test_file, int test_line,
                          ...);
extern intptr_t mock_(TestReporter *test_reporter, const char *function, const char *mock_file, int mock_line,
                      const char *parameters, ...);


/* Warning for unused results from 'when()' helps detecting slip-ups in the expect():

    expect(mocked_function, will_return(true)),
        when(param, is_equal_to(5));

   Note the extra parenthesis at the end of the first line. Thanks to
   the comma operator, this effectively creates a mock expectation
   which silently ignores the constraints on its parameter(s), and
   passes the test case. /@awilke

*/
extern Constraint *when_(const char *parameter, Constraint *constraint) WARN_UNUSED_RESULT;

extern Constraint *times_(int number_times_called);

extern void clear_mocks(void);
extern void tally_mocks(TestReporter *reporter);

#ifdef __cplusplus
    }
}
#endif

#endif
