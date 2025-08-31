#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Describe(LearningMocks);
BeforeEach(LearningMocks) {}
AfterEach(LearningMocks) {}

static int integer_out() {
    return (int)mock();
}

static char *string_out(int p1) {
    return (char *)mock(p1);
}

Ensure(LearningMocks, emit_pastable_code) {
    cgreen_mocks_are(learning_mocks);
    string_out(1);
    string_out(2);
    integer_out();
    integer_out();
    string_out(3);
    integer_out();
}
