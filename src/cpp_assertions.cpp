#include <cgreen/internal/cpp_assertions.h>
#include <cgreen/internal/c_assertions.h>
#include <cgreen/constraint.h>
#include <stdint.h>
#include <string.h>
#include <string>


#include "constraint_internal.h"

namespace cgreen {

    void assert_that_(const char *file, int line, const char *actual_string,
                      intptr_t actual, Constraint* constraint) {
        assert_core_(file, line, actual_string, actual, constraint);
    }

    void assert_that_(const char *file, int line, const char *actual_string,
                      double actual, Constraint* constraint) {
        assert_that_double_(file, line, actual_string, actual, constraint);
    }

    void assert_that_(const char *file, int line, const char *actual_string,
                      const std::string& actual, Constraint* constraint) {

        // if they are using a string constraint, they are almost certainly meaning to do a deep comparison
        if (is_string_comparing(constraint)) {
            assert_core_(file, line, actual_string, (intptr_t) (actual.c_str()),
                         constraint);
            return;
        }

        assert_that_(file, line, actual_string, (const std::string *) (&actual), constraint);
    }

    void assert_that_(const char *file, int line, const char *actual_string,
                      const std::string *actual, Constraint* constraint) {

        // if they are using a string constraint, they are almost certainly meaning to do a deep comparison
        if (is_string_comparing(constraint)) {
            assert_core_(file, line, actual_string, (intptr_t) (actual->c_str()),
                         constraint);
            return;
        }

        assert_core_(file, line, actual_string, (intptr_t) actual, constraint);
    }

}
