#ifndef UNIT_IMPLEMENTATION_HEADER
#define UNIT_IMPLEMENTATION_HEADER

typedef struct {
	const char* name;
	const char* filename;
	void (*setup)(void);
	void (*teardown)(void);
} CgreenContext;

typedef struct {
	CgreenContext* context;
	const char* name;
	void(*run)(void);
	const char* filename;
	int line;
} CgreenTest;


#define CGREEN_SPEC_PREFIX "CgreenSpec"
#define CGREEN_SEPARATOR "__"
#define spec_name(contextName, specName) CgreenSpec__##contextName##__##specName


#define Ensure_NARG(...) \
         Ensure_NARG_(__VA_ARGS__,Ensure_RSEQ_N())

#define Ensure_NARG_(...) \
         Ensure_ARG_N(__VA_ARGS__)

#define Ensure_ARG_N( \
          _1,_2,N,...) N

#define Ensure_RSEQ_N() \
        EnsureWithContextAndSpecificationName, EnsureWithSpecificationName, ENSURE_REQUIRES_NAME

#define EnsureWithContextAndSpecificationName(contextName, spec, ...) \
	static void contextName##_##spec (void);\
	CgreenTest spec_name(contextName, spec) = { &contextFor##contextName, #spec, &contextName##_##spec, __FILE__, __LINE__ };\
	static void contextName##_##spec ()

extern CgreenContext defaultContext;

#define EnsureWithSpecificationName(spec, ...) \
	static void spec (void);\
	CgreenTest spec_name(default, spec) = { &defaultContext, #spec, &spec, __FILE__, __LINE__ };\
	static void spec ()

#define DescribeImplementation(subject) \
		static void setup(void);				\
		static void teardown(void);										\
		static CgreenContext contextFor##subject = { #subject, __FILE__, &setup, &teardown }; \
		extern void(*BeforeEach_For_##subject)(void);					\
		extern void(*AfterEach_For_##subject)(void);					\
		static void setup(void) {										\
			if (BeforeEach_For_##subject != NULL) BeforeEach_For_##subject(); \
		}																\
		static void teardown(void) {									\
			if (AfterEach_For_##subject != NULL) AfterEach_For_##subject(); \
		}

#define BeforeEachImplementation(subject) \
		void BeforeEach_For_##subject##_Function(void);					\
		void(*BeforeEach_For_##subject)(void) = &BeforeEach_For_##subject##_Function; \
		void BeforeEach_For_##subject##_Function(void)

#define AfterEachImplementation(subject) \
		static void AfterEach_For_##subject##_Function(void);			\
		void(*AfterEach_For_##subject)(void) = &AfterEach_For_##subject##_Function; \
		static void AfterEach_For_##subject##_Function(void)

#endif
