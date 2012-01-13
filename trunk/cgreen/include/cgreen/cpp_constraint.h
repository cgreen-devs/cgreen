#include <cgreen/constraint.h>
#include <string>

namespace cgreen {

Constraint *create_equal_to_string_constraint(const std::string& expected_value, const char *expected_value_name);
Constraint *create_not_equal_to_string_constraint(const std::string& expected_value, const char *expected_value_name);
Constraint *create_contains_string_constraint(const std::string& expected_value, const char *expected_value_name);
Constraint *create_does_not_contain_string_constraint(const std::string& expected_value, const char *expected_value_name);
Constraint *create_equal_to_string_constraint(const std::string* expected_value, const char *expected_value_name);
Constraint *create_not_equal_to_string_constraint(const std::string* expected_value, const char *expected_value_name);
Constraint *create_contains_string_constraint(const std::string* expected_value, const char *expected_value_name);
Constraint *create_does_not_contain_string_constraint(const std::string* expected_value, const char *expected_value_name);

}
