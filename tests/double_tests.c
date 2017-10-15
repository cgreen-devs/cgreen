#include <cgreen/cgreen.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(Double);
BeforeEach(Double) {}
AfterEach(Double) {}

Ensure(Double, comparison_with_negative_values_should_not_always_succeed) {
    assert_that_double(-500, is_not_equal_to_double(0));
}
