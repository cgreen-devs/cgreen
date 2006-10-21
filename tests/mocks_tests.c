#include "../cgreen.h"
#include "../mocks.h"
#include <stdlib.h>

static int integer_in_integer_out(int i) {
    compare_integer(i);
    return (int)result();
}

static void can_stub_an_integer_return() {
    will_return(integer_in_integer_out, 3);
    assert_equal(integer_in_integer_out(0), 3, NULL);
}

TestSuite *mock_tests() {
    TestSuite *suite = create_test_suite();
    //add_test(suite, can_stub_an_integer_return);
    return suite;
}
