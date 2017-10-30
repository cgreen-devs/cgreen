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

xEnsure(Double, relative_epsilon_calculations_with_0_and_a_very_small_number_should_not_fail) {
    /* Issue #147 */
    assert_that_double(0.000000000000001, is_equal_to_double(0));
}

xEnsure(Double, comparisons_with_extremely_small_numbers_should_not_always_fail) {
    /*  Issue #149 */
    assert_that_double(4.9406564584124654E-324, is_equal_to_double(4.9406564584124654E-324));
}
