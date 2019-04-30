#include <cgreen/cgreen.h>
#include <cgreen/message_formatting.h>
#include <stdlib.h>
#include "src/constraint_internal.h"

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

    constraint->destroy(constraint);
    free(failure_message);
}

Ensure(MessageFormatting, shows_offset_as_zero_based) {
    char actual_data[] = {0x0a, 0x0b, 0x0c, 0x0d};
    char expected_data[] = {0x0b, 0x0b, 0x0c, 0x0d};
    Constraint *constraint =
        create_equal_to_contents_constraint(expected_data, 4, "expectd_data");

    char *failure_message = failure_message_for(constraint, "actual_data", (intptr_t)actual_data);
    assert_that(failure_message, contains_string("at offset"));
    assert_that(failure_message, contains_string("[0]"));
    assert_that(failure_message, contains_string("actual value:"));
    assert_that(failure_message, contains_string("expected value:"));
    assert_that(strstr(failure_message, "actual"), is_less_than(strstr(failure_message, "expected")));
    assert_that(failure_message, contains_string("0x0a"));
    assert_that(failure_message, contains_string("0x0b"));
    assert_that(strstr(failure_message, "0x0a"), is_less_than(strstr(failure_message, "0x0b")));

    constraint->destroy(constraint);
    free(failure_message);
    destroy_constraint(constraint);
}

TestSuite *message_formatting_tests(void) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, MessageFormatting, can_show_failure_message_containing_percent_sign);
    return suite;
}
