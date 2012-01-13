#include <cgreen/constraint.h>
#include <string>

namespace cgreen {

Constraint *is_equal_to_string(const std::string& value_to_match)
{
    return create_equal_to_string_constraint(value_to_match.c_str());
}

Constraint *is_not_equal_to_string(const std::string& value_to_match)
{
    return create_not_equal_to_string_constraint(value_to_match.c_str());
}

Constraint *contains_string(const std::string& value_to_match)
{
    return create_contains_string_constraint(value_to_match.c_str());
}

Constraint *does_not_contain_string(const std::string& value_to_match)
{
    return create_does_not_contain_string_constraint(value_to_match.c_str());
}

Constraint *is_equal_to_string(const std::string* value_to_match)
{
    return create_equal_to_string_constraint(value_to_match->c_str());
}

Constraint *is_not_equal_to_string(const std::string* value_to_match)
{
    return create_not_equal_to_string_constraint(value_to_match->c_str());
}

Constraint *contains_string(const std::string* value_to_match)
{
    return create_contains_string_constraint(value_to_match->c_str());
}

Constraint *does_not_contain_string(const std::string* value_to_match)
{
    return create_does_not_contain_string_constraint(value_to_match->c_str());
}

}
