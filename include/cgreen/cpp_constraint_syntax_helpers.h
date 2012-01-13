#ifndef CPP_CONSTRAINT_SYNTAX_HELPERS_HEADER
#define CPP_CONSTRAINT_SYNTAX_HELPERS_HEADER

#include <cgreen/constraint.h>
#include <string>

namespace cgreen {

Constraint *is_equal_to_string(const std::string& string_to_match);
Constraint *is_not_equal_to_string(const std::string& string_to_match);
Constraint *contains_string(const std::string& string_to_match);
Constraint *does_not_contain_string(const std::string& string_to_match);
Constraint *is_equal_to_string(const std::string* string_to_match);
Constraint *is_not_equal_to_string(const std::string* string_to_match);
Constraint *contains_string(const std::string* string_to_match);
Constraint *does_not_contain_string(const std::string* string_to_match);

}

#endif
