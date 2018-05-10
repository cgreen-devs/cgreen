#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/boxed_double.h>

#define PI 3.1415926

Describe(Doubles);
BeforeEach(Doubles) {}
AfterEach(Doubles) {}

Ensure(Doubles, can_assert_double_values) {
    significant_figures_for_assert_double_are(3);
    assert_that_double(3.14, is_equal_to_double(5.0));
}

static double double_out(int i, double d) {
    return unbox_double(mock(i, box_double(d))); // <1>
}

Ensure(Doubles, can_be_arguments_to_mock_and_returned) {
    expect(double_out,
           when(i, is_equal_to(15)),
           when(d, is_equal_to_double(31.32)), // <2>
           will_return_double(3.1415926));     // <3>
    assert_that_double(double_out(15, 31.32), is_equal_to_double(3.1415926));
}

int main(int argc, char **argv) {
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, Doubles, can_assert_double_values);
    add_test_with_context(suite, Doubles, can_be_arguments_to_mock_and_returned);
    return run_test_suite(suite, create_text_reporter());
}
