#ifndef ASSERTIONS_INTERNAL_HEADER
#define ASSERTIONS_INTERNAL_HEADER

#define assert_that_NARG(...) \
         assert_that_NARG_(__VA_ARGS__,assert_that_RSEQ_N())

#define assert_that_NARG_(...) \
         assert_that_ARG_N(__VA_ARGS__)

#define assert_that_ARG_N( \
          _1,_2,N,...) N

#define assert_that_RSEQ_N() \
        assert_that_constraint, assert_that_expression, ASSERT_THAT_REQUIRES_BOOLEAN_EXPRESSION_OR_ACTUAL_VALUE_AND_CONSTRAINT

#define assert_that_expression(expression) \
		assert_that_(__FILE__, __LINE__, #expression, expression, is_true);

void assert_equal_(const char *file, int line, const char *expression, intptr_t tried, intptr_t expected);
void assert_not_equal_(const char *file, int line, const char *expression, intptr_t tried, intptr_t expected);
void assert_double_equal_(const char *file, int line, const char *expression, double tried, double expected);
void assert_double_not_equal_(const char *file, int line, const char *expression, double tried, double expected);
void assert_string_equal_(const char *file, int line, const char *expression, const char *tried, const char *expected);
void assert_string_not_equal_(const char *file, int line, const char *expression, const char *tried, const char *expected);
void assert_that_double_(const char *file, int line, const char *actual_string, double actual, Constraint *constraint);

const char *show_null_as_the_string_null(const char *string);
bool doubles_are_equal(double tried, double expected);

#endif
