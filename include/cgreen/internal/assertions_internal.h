#ifndef ASSERTIONS_INTERNAL_HEADER
#define ASSERTIONS_INTERNAL_HEADER

#include <cgreen/filename.h>
#include <cgreen/string_comparison.h>

#ifdef __cplusplus
#include "cpp_assertions.h"
#endif
#include "c_assertions.h"
#include "stringify_token.h"


#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

//At the cost of duplication, these macros now give more descriptive error messages
#define ASSERT_THAT_VA_NUM_ARGS(...) ASSERT_THAT_VA_NUM_ARGS_IMPL_((__VA_ARGS__, _CALLED_WITH_TOO_MANY_ARGS,  _constraint,  _expression, DummyToFillVaArgs))
#define ASSERT_THAT_VA_NUM_ARGS_IMPL_(tuple) ASSERT_THAT_VA_NUM_ARGS_IMPL tuple

#define ASSERT_THAT_VA_NUM_ARGS_IMPL(_1, _2, _3, N, ...) N


#define ASSERT_THAT_macro_dispatcher(func, ...)   ASSERT_THAT_macro_dispatcher_(func, ASSERT_THAT_VA_NUM_ARGS(__VA_ARGS__))

#define ASSERT_THAT_macro_dispatcher_(func, nargs)           ASSERT_THAT_macro_dispatcher__(func, nargs)
#define ASSERT_THAT_macro_dispatcher__(func, nargs)           ASSERT_THAT_macro_dispatcher___(func, nargs)
#define ASSERT_THAT_macro_dispatcher___(func, nargs)          func ## nargs


#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#define assert_that_NARG(...) ASSERT_THAT_macro_dispatcher(assert_that, __VA_ARGS__)

#define assert_that_expression(expression) \
        assert_core_(FILENAME, __LINE__, STRINGIFY_TOKEN(expression), (expression), is_true);

void assert_equal_(const char *file, int line, const char *expression, intptr_t tried, intptr_t expected);
void assert_not_equal_(const char *file, int line, const char *expression, intptr_t tried, intptr_t expected);
void assert_double_equal_(const char *file, int line, const char *expression, double tried, double expected);
void assert_double_not_equal_(const char *file, int line, const char *expression, double tried, double expected);
void assert_string_equal_(const char *file, int line, const char *expression, const char *tried, const char *expected);
void assert_string_not_equal_(const char *file, int line, const char *expression, const char *tried, const char *expected);
void assert_that_double_(const char *file, int line, const char *actual_string, double actual, Constraint *constraint);

#ifdef __cplusplus
    }
}
#endif

#endif
