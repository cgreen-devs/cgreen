#include <cgreen/cgreen.h>
#include <cgreen/constraint.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>
#include <stdint.h>
#include <cgreen/cgreen_value.h>

#ifdef __cplusplus
using namespace cgreen;

namespace cgreen {
    extern "C" {
        extern CgreenValue make_cgreen_pointer_value(void *ptr);
    }
}

#else
extern CgreenValue make_cgreen_pointer_value(void *ptr);
#endif

Describe(MockStruct);
BeforeEach(MockStruct) {}
AfterEach(MockStruct) {}

typedef struct {
    int i;
    const char *string;
} Struct;


/* If you are only interested in a single, or few, fields: */
void function_mocking_field(Struct s) {
    mock(s.i);
}

Ensure(MockStruct, can_mock_a_struct_parameters_field) {
    Struct struct_to_send = { .i = 12, .string = "hello" };

    expect(function_mocking_field, when(s.i, is_equal_to(12)));

    function_mocking_field(struct_to_send);
}

void *cgreen_memdup(void *s, size_t size) {
    void *p = malloc(size);
    memcpy(p, s, size);
    return p;
}

#define memdup(s) cgreen_memdup(&s, sizeof(s))

/* If you need to get the whole struct: */
void function_mocking_the_whole_struct(Struct s) {
    Struct *sP = (Struct *)memdup(s);
    mock(sP);
}

Ensure(MockStruct, can_mock_a_struct_parameter) {
    Struct struct_to_send = { .i = 13, .string = "hello" };
    Struct *p;

    expect(function_mocking_the_whole_struct, will_capture_parameter(sP, p));

    function_mocking_the_whole_struct(struct_to_send);

    assert_that(p->i, is_equal_to(13));
}
