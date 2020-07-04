#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

void convert_to_uppercase(char *converted_string, const char *original_string) {
    mock(converted_string, original_string);
}

Ensure(setting_content_of_out_parameter) {
    expect(convert_to_uppercase,
           when(original_string, is_equal_to_string("upper case")),
           will_set_content_of_parameter(&converted_string,
                                         "UPPER CASE", 11));

}
