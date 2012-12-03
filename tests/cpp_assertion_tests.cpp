#include <cgreen/cgreen.h>
#include <stdexcept>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
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

static std::string* bob_pointer = NULL;
static std::string* alice_pointer = NULL;

void setup(void) {
//  for testing output of exceptions during setup
//	throw std::logic_error("this is why we fail");
}

Ensure(custom_large_classes_are_equal) {
    MyType my_class = { 3.1337f, 3.14159f, 90210 };
    MyType other = { 0.0f, 0.0f, 90210 };
    assert_that(my_class == other);
}

Ensure(custom_large_classes_are_not_equal) {
    MyType my_class = { 3.1337f, 3.14159f, 90210 };
    MyType other = { 3.1337f, 3.14159f, 90211 };
    assert_that(my_class != other);
}

Ensure(stl_string_references_are_not_equal) {
    std::string bob("bob");
    std::string alice("alice");
    assert_that(bob, is_not_equal_to_string(alice));
}

Ensure(stl_string_pointers_are_not_equal) {
    bob_pointer = new std::string("bob");
    alice_pointer = new std::string("alice");

    assert_that(bob_pointer, is_not_equal_to_string(alice_pointer));
}

Ensure(stl_string_pointer_and_reference_are_not_equal) {
    bob_pointer = new std::string("bob");
    std::string alice("alice");

    assert_that(bob_pointer, is_not_equal_to_string(alice));
}

Ensure(stl_string_reference_and_pointer_are_not_equal) {
    bob_pointer = new std::string("bob");
    std::string alice("alice");

    assert_that(alice, is_not_equal_to_string(bob_pointer));
}

Ensure(stl_string_pointer_and_reference_are_equal) {
    alice_pointer = new std::string("alice");
    std::string alice_reference("alice");

    assert_that(alice_pointer, is_equal_to_string(alice_reference));
}

Ensure(stl_string_reference_and_pointer_are_equal) {
    alice_pointer = new std::string("alice");
    std::string alice_reference("alice");

    assert_that(alice_reference, is_equal_to_string(alice_pointer));
}

Ensure(stl_string_pointer_is_not_null) {
    bob_pointer = new std::string("bob");

    assert_that(bob_pointer, is_non_null);
}

Ensure(stl_string_pointer_is_null) {
    std::string *null_string = (std::string *)NULL;
    assert_that(null_string, is_null);
}

// for quickly iterating on failure message formatting improvements
Ensure(stl_string_length_assertion_failure_is_readable) {
//    auto bob = new std::string("bob");
//    assert_that(bob->length(), is_not_equal_to(strlen("bob")));
}

Ensure(std_exception_what_is_output) {
//	throw std::logic_error("this is why we fail");
}

void teardown(void) {
	if (NULL != bob_pointer) {
		delete bob_pointer;
	}

	if (NULL != alice_pointer) {
		delete alice_pointer;
	}
}

TestSuite *cpp_assertion_tests() {
    TestSuite *suite = create_test_suite();
    set_setup(suite, setup);
    set_teardown(suite, teardown);
    add_test(suite, custom_large_classes_are_not_equal);
    add_test(suite, custom_large_classes_are_equal);
    add_test(suite, stl_string_references_are_not_equal);
    add_test(suite, stl_string_reference_and_pointer_are_not_equal);
    add_test(suite, stl_string_pointer_and_reference_are_not_equal);
    add_test(suite, stl_string_reference_and_pointer_are_equal);
    add_test(suite, stl_string_pointer_and_reference_are_equal);
    add_test(suite, stl_string_pointers_are_not_equal);
    add_test(suite, stl_string_pointer_is_not_null);
    add_test(suite, stl_string_pointer_is_null);
    add_test(suite, stl_string_length_assertion_failure_is_readable);
    add_test(suite, std_exception_what_is_output);
    return suite;
}
