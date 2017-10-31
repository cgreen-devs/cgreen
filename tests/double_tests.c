#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(Double);
BeforeEach(Double) {}
AfterEach(Double) {}

Ensure(Double, comparison_with_negative_values_should_not_always_succeed) {
    /*  Issue #146 - Fixed */
    assert_that_double(-500, is_not_equal_to_double(0));
}

Ensure(Double, comparisons_with_extremely_small_numbers_should_not_always_fail) {
    /*  Issue #149 */
    assert_that_double(4.9406564584124654E-324, is_equal_to_double(4.9406564584124654E-324));
}

Ensure(Double, numbers_within_absolute_tolerance_of_zero_are_considered_equal_to_zero) {
    double my_dbl_min = 2.2250738585072014e-308;
    assert_that_double(1.0e4 * my_dbl_min, is_equal_to_double(0.0));
}
