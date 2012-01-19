#include <cgreen/cpp_assertions.h>
#include <cgreen/constraint.h>
#include <inttypes.h>
#include <string.h>
#include <string>

namespace cgreen {

void assert_that_(const char *file, int line, const char *actual_string,
		const std::string& actual, Constraint* constraint) {

	// if they are using a string constraint, they are almost certainly meaning to do a deep comparison
	if (is_string_comparing(constraint)) {
		assert_that_(file, line, actual_string, (intptr_t) (actual.c_str()),
				constraint);
		return;
	}

	assert_that_(file, line, actual_string, (const std::string *) (&actual),
			constraint);
}

void assert_that_(const char *file, int line, const char *actual_string,
		const std::string *actual, Constraint* constraint) {

	// if they are using a string constraint, they are almost certainly meaning to do a deep comparison
	if (is_string_comparing(constraint)) {
		assert_that_(file, line, actual_string, (intptr_t) (actual->c_str()),
				constraint);
		return;
	}

	assert_that_(file, line, actual_string, (intptr_t) actual, constraint);
}

}
