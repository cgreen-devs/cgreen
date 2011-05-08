#ifndef CONSTRAINT_SYNTAX_HELPERS_HEADER
#define CONSTRAINT_SYNTAX_HELPERS_HEADER

#include <cgreen/constraint.h>
#include <stdint.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

/* we want to favor delegating functions (for namespacing, and to avoid confusing symbol/preprocessor conflicts), 
 * but for the intptr_t catch-all type, we need an explicit cast lest we get warnings-as-errors in newer compilers 
 */
#define is_equal_to(value) create_equal_to_value_constraint((intptr_t)value)
#define is_not_equal_to(value) create_not_equal_to_value_constraint((intptr_t)value)

#define is_equal_to_contents_of(pointer, size_of_contents) create_equal_to_contents_constraint((void *)pointer, size_of_contents)
#define is_not_equal_to_contents_of(pointer, size_of_contents) create_not_equal_to_contents_constraint((void *)pointer, size_of_contents)

#define will_return(value) create_return_value_constraint((intptr_t)value)
#define will_set_contents_of_parameter(parameter_name, value, size) create_set_parameter_value_constraint(#parameter_name, (intptr_t)value, (size_t)size)

Constraint *is_equal_to_string(const char *string_to_match);
Constraint *is_not_equal_to_string(const char *string_to_match);
Constraint *contains_string(const char *string_to_match);
Constraint *does_not_contain_string(const char *string_to_match);
Constraint *is_equal_to_double(double value_to_match);
Constraint *is_not_equal_to_double(double value_to_match);


/* these constraints don't take arguments, and we don't want to force users to put "()" on the end of every usage.
 * we also want to avoid macros when practical, for the namespacing and confusing symbol collision issues, 
 * so we use singleton instances.
 */
extern Constraint static_non_null_constraint;
extern Constraint *is_non_null;

extern Constraint static_null_constraint;
extern Constraint *is_null;

extern Constraint static_false_constraint;
extern Constraint *is_false;

extern Constraint static_true_constraint;
extern Constraint *is_true;

#ifdef __cplusplus
    }
}
#endif

#endif
