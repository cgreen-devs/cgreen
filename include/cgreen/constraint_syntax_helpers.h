#ifndef CONSTRAINT_SYNTAX_HELPERS_HEADER
#define CONSTRAINT_SYNTAX_HELPERS_HEADER


#include <cgreen/constraint.h>
#ifdef __cplusplus
#include <cgreen/cpp_constraint.h>
#endif
#include <stdint.h>

/* we normally want to favor delegating functions (for namespacing,
 * and to avoid confusing symbol/preprocessor conflicts), but for the
 * intptr_t catch-all type, we need an explicit cast lest we get
 * warnings-as-errors in newer compilers.  also, we need the textual
 * representation of the expected value and this is the only
 * reasonable way to do it.
 */

#ifdef __cplusplus
#define __CGREEN_NAMESPACE_PREFIX cgreen::
#else
#define __CGREEN_NAMESPACE_PREFIX /* empty */
#endif

#define is_equal_to(value) __CGREEN_NAMESPACE_PREFIX create_equal_to_value_constraint((intptr_t)(value), #value)
#define is_equal_to_hex(value) __CGREEN_NAMESPACE_PREFIX create_equal_to_hexvalue_constraint((intptr_t)(value), #value)
#define is_not_equal_to(value) __CGREEN_NAMESPACE_PREFIX create_not_equal_to_value_constraint((intptr_t)(value), #value)
#define is_non_null is_not_null
#define is_not_null __CGREEN_NAMESPACE_PREFIX create_not_null_constraint()
#define is_null __CGREEN_NAMESPACE_PREFIX create_is_null_constraint()
#define is_false __CGREEN_NAMESPACE_PREFIX create_is_false_constraint()
#define is_true __CGREEN_NAMESPACE_PREFIX create_is_true_constraint()

#define is_greater_than(value) __CGREEN_NAMESPACE_PREFIX create_greater_than_value_constraint((intptr_t)(value), #value)
#define is_less_than(value) __CGREEN_NAMESPACE_PREFIX create_less_than_value_constraint((intptr_t)(value), #value)

#define is_equal_to_contents_of(pointer, size_of_contents) __CGREEN_NAMESPACE_PREFIX create_equal_to_contents_constraint((void *)(pointer), size_of_contents, #pointer)
#define is_not_equal_to_contents_of(pointer, size_of_contents) __CGREEN_NAMESPACE_PREFIX create_not_equal_to_contents_constraint((void *)(pointer), size_of_contents, #pointer)

#define is_equal_to_string(value) __CGREEN_NAMESPACE_PREFIX create_equal_to_string_constraint(value, #value)
#define is_not_equal_to_string(value) __CGREEN_NAMESPACE_PREFIX create_not_equal_to_string_constraint(value, #value)
#define contains_string(value) __CGREEN_NAMESPACE_PREFIX create_contains_string_constraint(value, #value)
#define does_not_contain_string(value) __CGREEN_NAMESPACE_PREFIX create_does_not_contain_string_constraint(value, #value)
#define begins_with_string(value) __CGREEN_NAMESPACE_PREFIX create_begins_with_string_constraint(value, #value)
#define does_not_begin_with_string(value) __CGREEN_NAMESPACE_PREFIX create_does_not_begin_with_string_constraint(value, #value)
#define ends_with_string(value) __CGREEN_NAMESPACE_PREFIX create_ends_with_string_constraint(value, #value)
#define does_not_end_with_string(value) __CGREEN_NAMESPACE_PREFIX create_does_not_end_with_string_constraint(value, #value)

#define is_equal_to_double(value) __CGREEN_NAMESPACE_PREFIX create_equal_to_double_constraint(value, #value)
#define is_not_equal_to_double(value) __CGREEN_NAMESPACE_PREFIX create_not_equal_to_double_constraint(value, #value)

#define is_less_than_double(value) __CGREEN_NAMESPACE_PREFIX create_less_than_double_constraint(value, #value)
#define is_greater_than_double(value) __CGREEN_NAMESPACE_PREFIX create_greater_than_double_constraint(value, #value)

#define is_nearly_double(value) create_nearly_double_constraint(value, #value)

#define with_side_effect(callback, data) __CGREEN_NAMESPACE_PREFIX create_with_side_effect_constraint(callback, data)
#define will_return(value) __CGREEN_NAMESPACE_PREFIX create_return_value_constraint((intptr_t)(value))
#define will_return_by_value(value, size) __CGREEN_NAMESPACE_PREFIX create_return_by_value_constraint((intptr_t)&(value), size)
#define will_return_double(value) __CGREEN_NAMESPACE_PREFIX create_return_double_value_constraint(value)
#define will_set_contents_of_parameter(parameter_name, pointer_to_value, size) __CGREEN_NAMESPACE_PREFIX create_set_parameter_value_constraint(#parameter_name, (intptr_t)(pointer_to_value), (size_t)(size))
#define will_set_contents_of_output_parameter(parameter_name, pointer_to_value, size) __CGREEN_NAMESPACE_PREFIX create_set_parameter_value_constraint(#parameter_name, (intptr_t)(pointer_to_value), (size_t)(size))
#define will_capture_parameter(parameter_name, local_variable) __CGREEN_NAMESPACE_PREFIX create_capture_parameter_constraint(#parameter_name, &(local_variable), sizeof(local_variable))

#endif
