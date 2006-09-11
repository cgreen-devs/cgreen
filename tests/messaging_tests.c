#include "../cgreen.h"

void single_message_should_be_received() {
}

TestSuite *messaging_tests() {
    TestSuite *suite = create_test_suite();
    add_unit_test(suite, single_message_should_be_received);
    return suite;
}
