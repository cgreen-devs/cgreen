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
#define is_equal_to(value) cgreen::create_equal_to_value_constraint((intptr_t)value, #value)
#define is_equal_to_hex(value) cgreen::create_equal_to_hexvalue_constraint((intptr_t)value, #value)
#define is_not_equal_to(value) cgreen::create_not_equal_to_value_constraint((intptr_t)value, #value)
#define is_non_null is_not_null
#define is_not_null cgreen::create_not_null_constraint()
#define is_null cgreen::create_is_null_constraint()
#define is_false cgreen::create_is_false_constraint()
#define is_true cgreen::create_is_true_constraint()

#define is_greater_than(value) cgreen::create_greater_than_value_constraint((intptr_t)value, #value)
#define is_less_than(value) cgreen::create_less_than_value_constraint((intptr_t)value, #value)

#define is_equal_to_contents_of(pointer, size_of_contents) cgreen::create_equal_to_contents_constraint((void *)pointer, size_of_contents, #pointer)
#define is_not_equal_to_contents_of(pointer, size_of_contents) cgreen::create_not_equal_to_contents_constraint((void *)pointer, size_of_contents, #pointer)

#define is_equal_to_string(value) cgreen::create_equal_to_string_constraint(value, #value)
#define is_not_equal_to_string(value) cgreen::create_not_equal_to_string_constraint(value, #value)
#define contains_string(value) cgreen::create_contains_string_constraint(value, #value)
#define does_not_contain_string(value) cgreen::create_does_not_contain_string_constraint(value, #value)
#define begins_with_string(value) cgreen::create_begins_with_string_constraint(value, #value)
#define does_not_begin_with_string(value) cgreen::create_does_not_begin_with_string_constraint(value, #value)
#define ends_with_string(value) cgreen::create_ends_with_string_constraint(value, #value)
#define does_not_end_with_string(value) cgreen::create_does_not_end_with_string_constraint(value, #value)

#define is_equal_to_double(value) cgreen::create_equal_to_double_constraint(value, #value)
#define is_not_equal_to_double(value) cgreen::create_not_equal_to_double_constraint(value, #value)

#define is_less_than_double(value) cgreen::create_less_than_double_constraint(value, #value)
#define is_greater_than_double(value) cgreen::create_greater_than_double_constraint(value, #value)


#define with_side_effect(callback, data) cgreen::create_with_side_effect_constraint(callback, data)
#define will_return(value) cgreen::create_return_value_constraint((intptr_t)value)
#define will_return_by_value(value, size) cgreen::create_return_by_value_constraint((intptr_t)&value, size)
#define will_return_double(value) cgreen::create_return_double_value_constraint(value)
#define will_set_contents_of_parameter(parameter_name, pointer_to_value, size) cgreen::create_set_parameter_value_constraint(#parameter_name, (intptr_t)pointer_to_value, (size_t)size)
#define will_capture_parameter(parameter_name, local_variable) cgreen::create_capture_parameter_constraint(#parameter_name, &local_variable, sizeof(local_variable))
#else
#define is_equal_to(value) create_equal_to_value_constraint((intptr_t)value, #value)
#define is_equal_to_hex(value) create_equal_to_hexvalue_constraint((intptr_t)value, #value)
#define is_not_equal_to(value) create_not_equal_to_value_constraint((intptr_t)value, #value)
#define is_non_null is_not_null
#define is_not_null create_not_null_constraint()
#define is_null create_is_null_constraint()
#define is_false create_is_false_constraint()
#define is_true create_is_true_constraint()

#define is_greater_than(value) create_greater_than_value_constraint((intptr_t)value, #value)
#define is_less_than(value) create_less_than_value_constraint((intptr_t)value, #value)

#define is_equal_to_contents_of(pointer, size_of_contents) create_equal_to_contents_constraint((void *)pointer, size_of_contents, #pointer)
#define is_not_equal_to_contents_of(pointer, size_of_contents) create_not_equal_to_contents_constraint((void *)pointer, size_of_contents, #pointer)

#define is_equal_to_string(value) create_equal_to_string_constraint(value, #value)
#define is_not_equal_to_string(value) create_not_equal_to_string_constraint(value, #value)
#define contains_string(value) create_contains_string_constraint(value, #value)
#define does_not_contain_string(value) create_does_not_contain_string_constraint(value, #value)
#define begins_with_string(value) create_begins_with_string_constraint(value, #value)
#define does_not_begin_with_string(value) create_does_not_begin_with_string_constraint(value, #value)
#define ends_with_string(value) create_ends_with_string_constraint(value, #value)
#define does_not_end_with_string(value) create_does_not_end_with_string_constraint(value, #value)

#define is_equal_to_double(value) create_equal_to_double_constraint(value, #value)
#define is_not_equal_to_double(value) create_not_equal_to_double_constraint(value, #value)

#define is_less_than_double(value) create_less_than_double_constraint(value, #value)
#define is_greater_than_double(value) create_greater_than_double_constraint(value, #value)


#define with_side_effect(callback, data) create_with_side_effect_constraint(callback, data)
#define will_return(value) create_return_value_constraint((intptr_t)value)
#define will_return_by_value(value, size) create_return_by_value_constraint((intptr_t)&value, size)
#define will_return_double(value) create_return_double_value_constraint(value)
#define will_set_contents_of_parameter(parameter_name, pointer_to_value, size) create_set_parameter_value_constraint(#parameter_name, (intptr_t)pointer_to_value, (size_t)size)
#define will_capture_parameter(parameter_name, local_variable) create_capture_parameter_constraint(#parameter_name, &local_variable, sizeof(local_variable))
#endif

#endif
