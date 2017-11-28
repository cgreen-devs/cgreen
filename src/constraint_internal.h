#ifndef CONSTRAINT_INTERNAL_H
#define CONSTRAINT_INTERNAL_H

/* constraints internal functions are used from some user level tests so must be compilable in C++ */
#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

extern void destroy_empty_constraint(Constraint *constraint);
extern void destroy_static_constraint(Constraint *constraint);
extern void destroy_string_constraint(Constraint *constraint);
extern void destroy_double_constraint(Constraint *constraint);
extern void destroy_constraint(Constraint *);
extern void destroy_constraints(va_list constraints);


extern bool no_expected_value_in(const Constraint *constraint);
extern bool values_are_strings_in(const Constraint *constraint);
extern bool is_content_comparing(const Constraint *constraint);
extern bool is_content_setting(const Constraint *constraint);
extern bool is_not_content_setting(const Constraint *constraint);
extern bool is_string_comparing(const Constraint *constraint);
extern bool is_double_comparing(const Constraint *constraint);
extern bool is_comparing(const Constraint *constraint);
extern bool is_not_comparing(const Constraint *constraint);
extern bool is_parameter(const Constraint *);
extern bool constraint_is_not_for_parameter(const Constraint *, const char *);
extern bool constraint_is_for_parameter(const Constraint *, const char *);
extern bool constraint_is_for_parameter_in(const Constraint *, const char *);
extern bool doubles_are_equal(double tried, double expected);
extern bool double_is_lesser(double actual, double expected);
extern bool double_is_greater(double actual, double expected);


#ifdef __cplusplus
    }
}
#endif

#endif
