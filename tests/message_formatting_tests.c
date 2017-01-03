#include <cgreen/cgreen.h>
#include <cgreen/message_formatting.h>
#include <stdlib.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(MessageFormatting);
BeforeEach(MessageFormatting) {}
AfterEach(MessageFormatting) {}


Ensure(MessageFormatting, can_show_failure_message_containing_percent_sign) {
    const char *string_with_percent = "This contains %!";
    Constraint *constraint =
        create_equal_to_string_constraint(string_with_percent, "string_with_percent");

    char *failure_message = failure_message_for(constraint, "actual_string", (intptr_t)"This contains another %!");
    assert_that(failure_message, contains_string("contains %%!"));
    assert_that(failure_message, contains_string("another %%!"));

    free(failure_message);
    destroy_constraint(constraint);
}

TestSuite *message_formatting_tests() {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, MessageFormatting, can_show_failure_message_containing_percent_sign);
    return suite;
}
