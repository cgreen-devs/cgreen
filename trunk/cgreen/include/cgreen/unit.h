#ifndef UNIT_HEADER
#define UNIT_HEADER

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

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


#define Describe(subject) \
static void setup(void); \
static void teardown(void); \
static CgreenContext contextFor##subject = { #subject, __FILE__, &setup, &teardown }; \
extern void(*setupFor##subject##Pointer)(void); \
extern void(*teardownFor##subject##Pointer)(void); \
static void setup(void) { \
  if (setupFor##subject##Pointer != NULL) setupFor##subject##Pointer(); \
} \
static void teardown(void) { \
  if (teardownFor##subject##Pointer != NULL) teardownFor##subject##Pointer(); \
}

#define BeforeEach(subject) \
void setupFor##subject(void); \
void(*setupFor##subject##Pointer)(void) = &setupFor##subject; \
void setupFor##subject(void)

#define AfterEach(subject) \
static void teardownFor##subject(void); \
void(*teardownFor##subject##Pointer)(void) = &teardownFor##subject; \
static void teardownFor##subject(void)




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
	CgreenTest spec_name(contextName, spec) = { &contextFor##contextName, #spec, &contextName##_##spec, __FILE__, __LINE__ };\
	static void contextName##_##spec ()

extern CgreenContext defaultContext;

#define EnsureWithSpecificationName(spec, ...) \
	static void spec (void);\
	CgreenTest spec_name(default, spec) = { &defaultContext, #spec, &spec, __FILE__, __LINE__ };\
	static void spec ()

#ifdef __cplusplus
    }
}
#endif

#endif
