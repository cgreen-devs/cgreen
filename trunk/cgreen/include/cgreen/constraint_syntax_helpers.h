#ifndef CONSTRAINT_SYNTAX_HELPERS_HEADER
#define CONSTRAINT_SYNTAX_HELPERS_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/constraint.h>
#include <stdint.h>

/* we want to favor delegating functions (for namespacing, and to avoid confusing symbol/preprocessor conflicts), 
 * but for the intptr_t catch-all type, we need an explicit cast lest we get warnings-as-errors in newer compilers 
 */
#define is_equal_to(x) create_equal_to_intptr_constraint((intptr_t)x)
#define is_not_equal_to(x) create_not_equal_to_intptr_constraint((intptr_t)x)
#define will_return(x) create_return_value_constraint((intptr_t)x)

Constraint *is_equal_to_string(const char *value_to_match);
Constraint *is_not_equal_to_string(const char *value_to_match);
Constraint *contains_string(const char *value_to_match);
Constraint *does_not_contain_string(const char *value_to_match);
Constraint *is_equal_to_double(double value_to_match);
Constraint *is_not_equal_to_double(double value_to_match);


/* these constraints don't take arguments, and we don't want to force users to put "()" on the end of every usage.
 * we want to avoid macros, for the namespacing and confusing symbol collision issues, so we use singletons.
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
#endif

#endif
