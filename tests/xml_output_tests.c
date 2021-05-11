#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>

Ensure(failing_test_is_listed_by_xml_reporter) {
    assert_that(false);
}

Ensure(passing_test_is_listed_by_xml_reporter) {
    assert_that(true);
}

Ensure(error_message_gets_escaped_by_xml_reporter) {
    char *test_string =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n"
        "<example name=\"arbitrary xml content\">\n"
        "    <content>I'm the content & have chars which have to be escaped, "
        "if put in outer XML.</content>\n"
        "</example>";
    char *expected_string = "I'm not to be found!";
    assert_that(test_string, contains_string(expected_string));
}
