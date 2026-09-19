#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

struct structure {
    int field;
    char *string;
};

void update_field(struct structure *struct_to_update) {
    int *field = &struct_to_update->field;
    mock(struct_to_update, field);
}

Ensure(setting_field_of_parameter) {
    int fourty_two = 42;
    expect(update_field,
           will_set_contents_of_output_parameter(field, &fourty_two, sizeof(int)));
}
