#ifndef UNIT_HEADER
#define UNIT_HEADER

#include <cgreen/reporter.h>
#include <cgreen/suite.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#define spec_name(contextName, spec) CgreenSpec_##contextName##_##spec


#define Describe_NARG(...) \
         Describe_NARG_(__VA_ARGS__,Describe_RSEQ_N())

#define Describe_NARG_(...) \
         Describe_ARG_N(__VA_ARGS__)

#define Describe_ARG_N( \
          _1,_2,_3,N,...) N

#define Describe_RSEQ_N() \
        DescribeSubjectWithSetupAndTeardown, DescribeSubjectWithSetup, DescribeSubject, DESCRIBE_REQUIRES_SUBJECT

#define DescribeSubject(subject) \
static CgreenContext contextFor##subject = { #subject, __FILE__, NULL, NULL };

#define DescribeSubjectWithSetup(subject, ...) \
static void setup(void); \
static CgreenContext contextFor##subject = { #subject, __FILE__, &setup, NULL };

#define DescribeSubjectWithSetupAndTeardown(subject, ...) \
static void setup(void); \
static void teardown(void); \
static CgreenContext contextFor##subject = { #subject, __FILE__, &setup, &teardown };

#define Describe(...) Describe_NARG(__VA_ARGS__)(__VA_ARGS__)

#define Ensure_NARG(...) \
         Ensure_NARG_(__VA_ARGS__,Ensure_RSEQ_N())

#define Ensure_NARG_(...) \
         Ensure_ARG_N(__VA_ARGS__)

#define Ensure_ARG_N( \
          _1,_2,N,...) N

#define Ensure_RSEQ_N() \
        EnsureWithContextAndSpecificationName, EnsureWithSpecificationName, ENSURE_REQUIRES_NAME

#define Ensure(...) Ensure_NARG(__VA_ARGS__)(__VA_ARGS__)

#define EnsureWithContextAndSpecificationName(contextName, spec, ...) \
	static void contextName##_##spec (void);\
	CgreenTest spec_name(contextName, spec) = { &contextFor##contextName, #spec, &contextName##_##spec };\
	static void contextName##_##spec ()

#define EnsureWithSpecificationName(spec, ...) \
	static void spec (void);\
	CgreenTest spec_name(default, spec) = { &defaultContext, #spec, &spec };\
	static void spec ()

////////////////////////////////////////////////////////////////////////////////

void die_in(unsigned int seconds);

#ifdef __cplusplus
    }
}
#endif

#endif
