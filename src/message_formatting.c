#include <cgreen/constraint.h>
#include <cgreen/message_formatting.h>
#include <cgreen/string_comparison.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

static int find_index_of_difference(char *expected, char *actual, size_t size_to_compare) {
    char *expectedp = expected;
    char *actualp = actual;

    while (size_to_compare--) {
        if (*expectedp++ != *actualp++) {
            return actualp - actual;
        }
    }

    return -1;
}

static bool actual_value_not_necessary_for(Constraint *constraint, const char *actual_string, const char *actual_value_string) {
  (void)(constraint); // UNUSED!
	return strings_are_equal(actual_string, actual_value_string) ||
            strings_are_equal(actual_string, "true") ||
            strings_are_equal(actual_string, "false");
}


bool parameters_are_not_valid_for(Constraint *constraint, intptr_t actual) {
	char message[10] = {'\0'};
	format_validation_failure_message_for(message, sizeof(message), "", constraint, actual);

	return strlen(message) > 0;
}

void format_validation_failure_message_for(char *message, size_t message_size, const char *function, Constraint *constraint, intptr_t actual) {
  (void)(function); // UNUSED!

//	if (function != NULL && strlen(function) > 0) {
//		snprintf(message, message_size - 1, "\tIn mocked function [%s]:\n", function);
//	} else {
//		snprintf(message, message_size - 1, "\tIn assertion:\n");
//	}

	if (is_content_comparing(constraint)) {
		const char *compared_to_name;
		if (constraint->parameter_name != NULL) {
			compared_to_name = constraint->parameter_name;
		} else {
			compared_to_name = constraint->expected_value_name;
		}

		if (constraint->size_of_expected_value <= 0) {
			snprintf(message + strlen(message), message_size - strlen(message) - 1,
					"\t\tWanted to compare contents with [%s], but [%Zd] was given for the comparison size.\n",
						compared_to_name,
						constraint->size_of_expected_value);
			return;
		}

		if ((void *)actual == NULL) {
			snprintf(message + strlen(message), message_size - strlen(message) - 1,
					"\t\tWanted to compare contents with [%s], but NULL was for the pointer we wanted to compare to.\n"
					"\t\tIf you want to explicitly check for null, use the is_null constraint instead.\n",
						compared_to_name);

				return;
		}

		if ((void *)constraint->expected_value == NULL) {
			snprintf(message + strlen(message), message_size - strlen(message) - 1,
					"\t\tWanted to compare contents of [%s] but it had a value of NULL.\n"
					"\t\tIf you want to explicitly check for null, use the is_null constraint instead.\n",
						compared_to_name);

				return;
		}
	}
}

void format_expectation_failure_message_for(char *message, size_t message_size, Constraint *constraint, const char *actual_string, intptr_t actual) {
    char actual_value_string[32];
    snprintf(actual_value_string, sizeof(actual_value_string) - 1, "%" PRIdPTR, actual);

    snprintf(message, message_size - 1,
            "Expected [%s] to [%s]",
            actual_string,
            constraint->name);

    if (no_expected_value_in(constraint)) {
        strcat(message, "\n");
        return;
    } else
        strcat(message, " ");

    snprintf(message + strlen(message), message_size - strlen(message) - 1,
            "[%s]\n",
            constraint->expected_value_name);

    if (actual_value_not_necessary_for(constraint, actual_string, actual_value_string)) {
        /* when the actual string and the actual value are the same, don't print both of them */
        /* also, don't print "0" for false and "1" for true */
    	/* also, don't print expected/actual for contents constraints since that is useless */
        return;
    }

    /* for string constraints, print out the strings encountered and not their pointer values */
    if (values_are_strings_in(constraint)) {
		snprintf(message + strlen(message), message_size - strlen(message) - 1,
					"\t\tactual value:\t[\"%s\"]\n"
					"\t\texpected value:\t[\"%s\"]\n",
					(const char *)actual,
					(const char *)constraint->expected_value);
		return;
    }

    if(is_content_comparing(constraint)) {
        int difference_index = find_index_of_difference((char *)constraint->expected_value, (char *)actual, constraint->size_of_expected_value);
		snprintf(message + strlen(message), message_size - strlen(message) - 1,
				"\t\tat offset:\t[%d]\n",
				difference_index);
		return;
    }

    snprintf(message + strlen(message), message_size - strlen(message) - 1,
	     "\t\tactual value:\t[%" PRIdPTR "]\n",
	     actual);

    if(strstr(constraint->name, "not ") == NULL) {
	snprintf(message + strlen(message), message_size - strlen(message) - 1,
		 "\t\texpected value:\t[%" PRIdPTR "]\n",
		 constraint->expected_value);
    }
}

#ifdef __cplusplus
}
#endif
