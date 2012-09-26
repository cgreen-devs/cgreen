#ifndef CPP_ASSERTIONS_HEADER
#define CPP_ASSERTIONS_HEADER

#include <cgreen/constraint.h>
#include <inttypes.h>
#include <string>
#include <typeinfo>

namespace cgreen {

	#define assert_that_constraint(actual, constraint) assert_that_(__FILE__, __LINE__, #actual, actual, constraint)

	void assert_that_(const char *file, int line, const char *actual_string, const std::string& actual, Constraint *constraint);
	void assert_that_(const char *file, int line, const char *actual_string, const std::string *actual, Constraint *constraint);

	// this isn't declared in assertions.h because you can't have overloads for an extern "C"-declared function, so it seems
	void assert_that_(const char *file, int line, const char *actual_string, intptr_t actual, Constraint *constraint);

	template <typename T> void assert_that_(const char *file, int line, const char *actual_string, T actual, Constraint *constraint) {

		if (typeid(actual) == typeid(std::string&) ||
			typeid(actual) == typeid(const std::string&) ||
			typeid(actual) == typeid(const std::string*) ||
			typeid(actual) == typeid(std::string*)) {

			assert_that_(file, line, actual_string, reinterpret_cast<const std::string *>(actual), constraint);

		} else if (typeid(actual) == typeid(std::string)) {

			assert_that_(file, line, actual_string, (const std::string *)&actual, constraint);

		} else {
			assert_that_(file, line, actual_string, (intptr_t)actual, constraint);
		}
	}
}

#endif
