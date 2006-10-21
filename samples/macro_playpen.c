#include "../mocks.h"

char *a_mock(char *s, int i, void *a) {
    compare_string(s);
    compare_integer(i);
    compare_address(a);
    return (char *)result();
}

void a_test() {
    expect(f, 1, '2', "3", ANYTHING);
    always expect(f, 1, '2', "3", ANYTHING);
    returns(f, 37);
    mock(f, "Gives", 1, '2', "3", ANYTHING);
}
