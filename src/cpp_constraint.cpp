#include <cgreen/constraint.h>
#include <string>

namespace cgreen {

CgreenConstraint *create_equal_to_string_constraint(const std::string& expected_value, const char *expected_value_name)
{
    return create_equal_to_string_constraint(expected_value.c_str(), expected_value_name);
}

CgreenConstraint *create_equal_to_string_constraint(const std::string* expected_value, const char *expected_value_name)
{
    return create_equal_to_string_constraint(expected_value->c_str(), expected_value_name);
}

CgreenConstraint *create_not_equal_to_string_constraint(const std::string& expected_value, const char *expected_value_name)
{
    return create_not_equal_to_string_constraint(expected_value.c_str(), expected_value_name);
}

CgreenConstraint *create_not_equal_to_string_constraint(const std::string* expected_value, const char *expected_value_name)
{
    return create_not_equal_to_string_constraint(expected_value->c_str(), expected_value_name);
}

CgreenConstraint *create_contains_string_constraint(const std::string& expected_value, const char *expected_value_name)
{
    return create_contains_string_constraint(expected_value.c_str(), expected_value_name);
}

CgreenConstraint *create_contains_string_constraint(const std::string* expected_value, const char *expected_value_name)
{
    return create_contains_string_constraint(expected_value->c_str(), expected_value_name);
}

CgreenConstraint *create_does_not_contain_string_constraint(const std::string& expected_value, const char *expected_value_name)
{
    return create_does_not_contain_string_constraint(expected_value.c_str(), expected_value_name);
}

CgreenConstraint *create_does_not_contain_string_constraint(const std::string* expected_value, const char *expected_value_name)
{
    return create_does_not_contain_string_constraint(expected_value->c_str(), expected_value_name);
}

CgreenConstraint *create_begins_with_string_constraint(const std::string& expected_value, const char *expected_value_name)
{
    return create_begins_with_string_constraint(expected_value.c_str(), expected_value_name);
}

CgreenConstraint *create_begins_with_string_constraint(const std::string* expected_value, const char *expected_value_name)
{
    return create_begins_with_string_constraint(expected_value->c_str(), expected_value_name);
}

}
