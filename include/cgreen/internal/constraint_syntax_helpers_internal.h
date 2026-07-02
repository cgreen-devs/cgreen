#ifndef CONSTRAINT_SYNTAX_HELPERS_INTERNAL_HEADER
#define CONSTRAINT_SYNTAX_HELPERS_INTERNAL_HEADER

/* Argument-count dispatch so that will_return_by_value() can be called either
   with an explicit size or with just the value (the size is then derived with
   sizeof). This mirrors the assert_that() dispatch in assertions_internal.h and
   is kept out of constraint_syntax_helpers.h to keep that file readable.

   Both branches call the same create_return_by_value_constraint(). */

#define will_return_by_value_NARG(...) \
    WILL_RETURN_BY_VALUE_dispatcher(will_return_by_value, __VA_ARGS__)

#define WILL_RETURN_BY_VALUE_dispatcher(func, ...) \
    WILL_RETURN_BY_VALUE_dispatcher_(func, WILL_RETURN_BY_VALUE_VA_NUM_ARGS(__VA_ARGS__))
#define WILL_RETURN_BY_VALUE_dispatcher_(func, nargs)  WILL_RETURN_BY_VALUE_dispatcher__(func, nargs)
#define WILL_RETURN_BY_VALUE_dispatcher__(func, nargs) func ## nargs

#define WILL_RETURN_BY_VALUE_VA_NUM_ARGS(...) \
    WILL_RETURN_BY_VALUE_VA_NUM_ARGS_IMPL_((__VA_ARGS__, _too_many_args, _with_size, _sized_by_type, dummy))
#define WILL_RETURN_BY_VALUE_VA_NUM_ARGS_IMPL_(tuple) WILL_RETURN_BY_VALUE_VA_NUM_ARGS_IMPL tuple
#define WILL_RETURN_BY_VALUE_VA_NUM_ARGS_IMPL(_1, _2, _3, N, ...) N

#define will_return_by_value_sized_by_type(value) \
    __CGREEN_NAMESPACE_PREFIX create_return_by_value_constraint((intptr_t)&(value), sizeof(value))
#define will_return_by_value_with_size(value, size) \
    __CGREEN_NAMESPACE_PREFIX create_return_by_value_constraint((intptr_t)&(value), (size))

#endif
