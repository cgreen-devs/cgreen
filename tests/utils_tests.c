#include <cgreen/cgreen.h>

#include "../src/utils.h"

Describe(Utils);
BeforeEach(Utils) {}
AfterEach(Utils) {}


Ensure(Utils, panic_output_contains_message) {
    char message[] = "some error occurred";
    char buffer[100];
    panic_set_output_buffer(buffer);
    PANIC(message);
    assert_that(buffer, contains_string(message));
}

Ensure(Utils, panic_output_contains_filename) {
    char buffer[100];
    panic_set_output_buffer(buffer);
    PANIC("");
    assert_that(buffer, contains_string(__FILE__));
}
