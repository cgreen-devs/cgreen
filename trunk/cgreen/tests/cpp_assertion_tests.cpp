#include <cgreen/cgreen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>

using namespace cgreen;

class MyType
{
public:
    double x;
    double y;
    uint64_t z;
    bool operator==(MyType& other)
    {
        return this->z == other.z;
    }
    bool operator!=(MyType& other)
    {
        return this->z != other.z;
    }
};

#define assert_that_type(actual, expected) \
  assert_true_with_message(actual expected, "Expected [%s] to be [%s]", #actual, #expected)

#define is_equal_to_type(type) ==type
#define is_not_equal_to_type(type) !=type

Ensure(custom_large_classes_are_equal) {
    MyType my_type = { 3.1337f, 3.14159f, 90210 };
    MyType other = { 0.0f, 0.0f, 90210 };
    assert_that_type(my_type, is_equal_to_type(other));
}

Ensure(custom_large_classes_are_not_equal) {
    MyType my_type = { 3.1337f, 3.14159f, 90210 };
    MyType other = { 3.1337f, 3.14159f, 90211 };
    assert_that_type(my_type, is_not_equal_to_type(other));
}

TestSuite *cpp_assertion_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, custom_large_classes_are_not_equal);
    add_test(suite, custom_large_classes_are_equal);
    return suite;
}
