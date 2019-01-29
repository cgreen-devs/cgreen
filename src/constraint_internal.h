#ifndef CONSTRAINT_INTERNAL_H
#define CONSTRAINT_INTERNAL_H

/* constraints internal functions are used from some user level tests so must be compilable in C++ */
#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

extern void destroy_empty_constraint(CgreenConstraint *constraint);
extern void destroy_static_constraint(CgreenConstraint *constraint);
extern void destroy_string_constraint(CgreenConstraint *constraint);
extern void destroy_double_constraint(CgreenConstraint *constraint);
extern void destroy_constraint(CgreenConstraint *);
extern void destroy_constraints(va_list constraints);


extern bool no_expected_value_in(const CgreenConstraint *constraint);
extern bool values_are_strings_in(const CgreenConstraint *constraint);
extern bool is_content_comparing(const CgreenConstraint *constraint);
extern bool is_content_setting(const CgreenConstraint *constraint);
extern bool is_not_content_setting(const CgreenConstraint *constraint);
extern bool is_string_comparing(const CgreenConstraint *constraint);
extern bool is_double_comparing(const CgreenConstraint *constraint);
extern bool is_comparing(const CgreenConstraint *constraint);
extern bool is_not_comparing(const CgreenConstraint *constraint);
extern bool is_parameter(const CgreenConstraint *);
extern bool constraint_is_not_for_parameter(const CgreenConstraint *, const char *);
extern bool constraint_is_for_parameter(const CgreenConstraint *, const char *);
extern bool constraint_is_for_parameter_in(const CgreenConstraint *, const char *);
extern bool doubles_are_equal(double tried, double expected);
extern bool double_is_lesser(double actual, double expected);
extern bool double_is_greater(double actual, double expected);


#ifdef __cplusplus
    }
}
#endif

#endif
