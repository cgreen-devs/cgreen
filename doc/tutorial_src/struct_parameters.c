#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

Describe(StructParameters);
BeforeEach(StructParameters) {}
AfterEach(StructParameters) {}

/*
  This is uncompilable code that is inserted as a non-example

typedef struct {
    int i;
    char *string;
} Struct;

int function_taking_struct(Struct s) {
    return (int)mock(?);
}
*/

typedef struct {
    int i;
    char *string;
} Struct;

int function_checking_a_field(Struct s) {
    return (int)mock(s.i);
}


Ensure(StructParameters, can_mock_field_in_parameter) {
    Struct s = { .i = 12, .string = "hello" };

    expect(function_checking_a_field, when(s.i, is_equal_to(12)),
           will_return(12));

    assert_that(function_checking_a_field(s), is_equal_to(12));

}

int function_checking_multiple_fields(Struct s) {
    return (int)mock(s.i, s.string);
}

Ensure(StructParameters, can_mock_muultiple_fields_in_parameter) {
    Struct s = { .i = 13, .string = "hello world!" };

    expect(function_checking_multiple_fields,
           when(s.i, is_equal_to(13)),
           when(s.string, begins_with_string("hello")),
           will_return(13));

    assert_that(function_checking_multiple_fields(s), is_equal_to(13));
}
