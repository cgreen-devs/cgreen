#include <cgreen/cgreen.h>

static int convert_roman_to_decimal(char *roman) {
    return 0;
}

Describe(Converter);
BeforeEach(Converter) {}
AfterEach(Converter) {}

Ensure(Converter, converts_XIV_to_14) {
    assert_that(convert_roman_to_decimal("XIV"), is_equal_to(14));
}
