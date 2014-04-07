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
#define spec_name(contextName, testName) CgreenSpec__##contextName##__##testName##__

//This gives better error messages at the cost of duplication
#define ENSURE_VA_NUM_ARGS(...) ENSURE_VA_NUM_ARGS_IMPL_((__VA_ARGS__, _CALLED_WITH_TOO_MANY_ARGUMENTS,  WithContextAndSpecificationName,  WithSpecificationName))
#define ENSURE_VA_NUM_ARGS_IMPL_(tuple) ENSURE_VA_NUM_ARGS_IMPL tuple

#define ENSURE_VA_NUM_ARGS_IMPL(_1, _2, _3, N, ...) N

#define ENSURE_macro_dispatcher(func, ...)   ENSURE_macro_dispatcher_(func, ENSURE_VA_NUM_ARGS(__VA_ARGS__))

// these levels of indirecton are a work-around for variadic macro deficiencies in Visual C++ 2012 and prior
#define ENSURE_macro_dispatcher_(func, nargs)           ENSURE_macro_dispatcher__(func, nargs)
#define ENSURE_macro_dispatcher__(func, nargs)           ENSURE_macro_dispatcher___(func, nargs)
#define ENSURE_macro_dispatcher___(func, nargs)          func ## nargs

#define Ensure_NARG(...) ENSURE_macro_dispatcher(Ensure, __VA_ARGS__)

#define EnsureWithContextAndSpecificationName(contextName, spec, ...) \
    static void contextName##__##spec (void);\
    CgreenTest spec_name(contextName, spec) = { &contextFor##contextName, #spec, &contextName##__##spec, __FILE__, __LINE__ };\
    static void contextName##__##spec (void)

extern CgreenContext defaultContext;

#define EnsureWithSpecificationName(spec, ...) \
    static void spec (void);\
    CgreenTest spec_name(default, spec) = { &defaultContext, #spec, &spec, __FILE__, __LINE__ };\
    static void spec (void)

#define DescribeImplementation(subject) \
        static void setup(void);                \
        static void teardown(void);                                     \
        static CgreenContext contextFor##subject = { #subject, __FILE__, &setup, &teardown }; \
        extern void(*BeforeEach_For_##subject)(void);                   \
        extern void(*AfterEach_For_##subject)(void);                    \
        static void setup(void) {                                       \
            if (BeforeEach_For_##subject != NULL) BeforeEach_For_##subject(); \
        }                                                               \
        static void teardown(void) {                                    \
            if (AfterEach_For_##subject != NULL) AfterEach_For_##subject(); \
        }

#define BeforeEachImplementation(subject) \
        void BeforeEach_For_##subject##_Function(void);                 \
        void(*BeforeEach_For_##subject)(void) = &BeforeEach_For_##subject##_Function; \
        void BeforeEach_For_##subject##_Function(void)

#define AfterEachImplementation(subject) \
        static void AfterEach_For_##subject##_Function(void);           \
        void(*AfterEach_For_##subject)(void) = &AfterEach_For_##subject##_Function; \
        static void AfterEach_For_##subject##_Function(void)

#endif
