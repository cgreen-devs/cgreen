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

#define assert_that_class(actual, expected) \
  assert_true_with_message(actual expected, "Expected [%s] to be [%s]", #actual, #expected)

#define is_equal_to_type(type) ==type
#define is_not_equal_to_type(type) !=type

Ensure(custom_large_classes_are_equal) {
    MyType my_class = { 3.1337f, 3.14159f, 90210 };
    MyType other = { 0.0f, 0.0f, 90210 };
    assert_that_class(my_class, is_equal_to_type(other));
}

Ensure(custom_large_classes_are_not_equal) {
    MyType my_class = { 3.1337f, 3.14159f, 90210 };
    MyType other = { 3.1337f, 3.14159f, 90211 };
    assert_that_class(my_class, is_not_equal_to_type(other));
}

Ensure(stl_string_references_are_not_equal) {
    std::string bob("bob");
    std::string alice("alice");
    assert_that(bob, is_not_equal_to_string(alice));
}

Ensure(stl_string_pointers_are_not_equal) {
    auto bob = new std::string("bob");
    auto alice = new std::string("alice");
    assert_that(bob, is_not_equal_to_string(alice));
}

Ensure(stl_string_pointer_and_reference_are_not_equal) {
    auto bob = new std::string("bob");
    std::string alice("alice");
    assert_that(bob, is_not_equal_to_string(alice));
}

Ensure(stl_string_reference_and_pointer_are_not_equal) {
    auto bob = new std::string("bob");
    std::string alice("alice");
    assert_that(alice, is_not_equal_to_string(bob));
}

Ensure(stl_string_pointer_and_reference_are_equal) {
    auto alice_pointer = new std::string("alice");
    std::string alice_reference("alice");
    assert_that(alice_pointer, is_equal_to_string(alice_reference));
}

Ensure(stl_string_reference_and_pointer_are_equal) {
    auto alice_pointer = new std::string("alice");
    std::string alice_reference("alice");
    assert_that(alice_reference, is_equal_to_string(alice_pointer));
}

Ensure(stl_string_pointer_is_not_null) {
    auto bob = new std::string("bob");
    assert_that(bob, is_non_null);
}

TestSuite *cpp_assertion_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, custom_large_classes_are_not_equal);
    add_test(suite, custom_large_classes_are_equal);
    add_test(suite, stl_string_references_are_not_equal);
    add_test(suite, stl_string_reference_and_pointer_are_not_equal);
    add_test(suite, stl_string_pointer_and_reference_are_not_equal);
    add_test(suite, stl_string_reference_and_pointer_are_equal);
    add_test(suite, stl_string_pointer_and_reference_are_equal);
    add_test(suite, stl_string_pointers_are_not_equal);
    add_test(suite, stl_string_pointer_is_not_null);
    return suite;
}
