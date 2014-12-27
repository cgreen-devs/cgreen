#include <cgreen/constraint.h>
#include <cgreen/message_formatting.h>
#include <cgreen/string_comparison.h>
#include <inttypes.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include "wincompat.h"
#endif

#ifdef __cplusplus
namespace cgreen {
#endif

static int find_index_of_difference(char *expected, char *actual, size_t size_to_compare) {
    char *expectedp = expected;
    char *actualp = actual;

    while (size_to_compare--) {
        if (*expectedp++ != *actualp++) {
            return (int)(actualp - actual);
        }
    }

    return -1;
}

static bool actual_value_not_necessary_for(Constraint *constraint, const char *actual_string, const char *actual_value_string) {
    (void)constraint; // UNUSED!
    return strings_are_equal(actual_string, actual_value_string) ||
            strings_are_equal(actual_string, "true") ||
            strings_are_equal(actual_string, "false");
}


bool parameters_are_not_valid_for(Constraint *constraint, intptr_t actual) {
    char *message = validation_failure_message_for(constraint, actual);

    bool not_valid = (strlen(message) > 0);

    free(message);

    return not_valid;
}

char *validation_failure_message_for(Constraint *constraint, intptr_t actual) {
    const char *name_has_incorrect_size_message =
    		"\t\tWanted to compare contents with [%s], but [%lu] was given for the comparison size.";
    const char *null_used_for_compare_message =
            "\t\tWanted to compare contents with [%s], but NULL was used for the pointer we wanted to compare to."
            "\t\tIf you want to explicitly check for null, use the is_null constraint instead.";
    const char *null_used_for_actual_message =
            "\t\tWanted to compare contents of [%s] but it had a value of NULL.\n"
            "\t\tIf you want to explicitly check for null, use the is_null constraint instead.";



    size_t message_size = strlen(name_has_incorrect_size_message) +
    		strlen(null_used_for_compare_message) +
    		strlen(null_used_for_actual_message) +
    		512; // just in case

    char *message = (char *)malloc(message_size);
    memset(message, 0, message_size);

//    (void)function; // UNUSED!
//  if (function != NULL && strlen(function) > 0) {
//      snprintf(message, message_size - 1, "\tIn mocked function [%s]:\n", function);
//  } else {
//      snprintf(message, message_size - 1, "\tIn assertion:\n");
//  }

    if (is_content_comparing(constraint)) {
        const char *compared_to_name;
        if (constraint->parameter_name != NULL) {
            compared_to_name = constraint->parameter_name;
        } else {
            compared_to_name = constraint->expected_value_name;
        }

        if (constraint->size_of_expected_value <= 0) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                    name_has_incorrect_size_message,
                    compared_to_name,
                    (unsigned long)constraint->size_of_expected_value);

            return message;
        }

        if ((void *)actual == NULL) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
            		null_used_for_compare_message,
                    compared_to_name);

                return message;
        }

        if ((void *)constraint->expected_value == NULL) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
            		null_used_for_actual_message,
                    compared_to_name);

                return message;
        }
    }

    return message;
}


static char *next_percent_sign(const char *s) {
    return strchr(s, '%');
}

size_t count_percent_signs(char const *s)
{
    size_t count = 0;
    char const *p = next_percent_sign(s);
    while (p != NULL) {
        count++;
        p = next_percent_sign(p+1);
    }
    return count;
}


char *copy_while_doubling_percent_signs(char *string, char const *original)
{
    char *destination = string;
    const char *source = original;
    const char *next = next_percent_sign(source);
    for (; next != NULL; next = next_percent_sign(next+1)) {
        size_t len = next - source + 1;
        memcpy(destination, source, len);
        destination += len;
        *destination++ = '%';
        source = next+1;
    }
    strcpy(destination, source);

    return string;
}


void double_all_percent_signs_in(char **original)
{
    size_t percent_count = count_percent_signs(*original);
    if (percent_count == 0) {
        return;
    }
    char *new_string = (char *)malloc(strlen(*original) + percent_count + 1);
    if (new_string == NULL) {
        return;
    }
    copy_while_doubling_percent_signs(new_string, *original);
    free(*original);
    *original = new_string;
}


char *failure_message_for(Constraint *constraint, const char *actual_string, intptr_t actual) {
    char actual_value_string[32];
    const char *actual_value_constraint_name = "Expected [%s] to [%s]";
    const char *expected_value_name =  "[%s]\n";
    const char *actual_value_as_string = "\t\tactual value:\t\t\t[\"%s\"]";
    const char *at_offset = "\t\tat offset:\t\t\t[%d]";
    const char *actual_value_message = "\t\tactual value:\t\t\t[%" PRIdPTR "]";
    char *message;
    size_t message_size = strlen(actual_value_constraint_name) +
    		strlen(expected_value_name) +
    		strlen(actual_value_as_string) +
    		strlen(at_offset) +
    		strlen(actual_value_message) +
    		strlen(constraint->expected_value_message) +
    		strlen(constraint->expected_value_name) +
    		strlen(constraint->name) +
    		strlen(actual_string) +
    		512; // just in case

    snprintf(actual_value_string, sizeof(actual_value_string) - 1, "%" PRIdPTR, actual);

    if (values_are_strings_in(constraint)) {
    	message_size += strlen((char *)constraint->expected_value);
    	if (actual != (intptr_t)NULL) {
    		message_size += strlen((char *)actual);
    	}
    }

    message = (char *)malloc(message_size);

    snprintf(message, message_size - 1,
    		actual_value_constraint_name,
    		actual_string,
            constraint->name);

    if (no_expected_value_in(constraint)) {
        return message;
    } else
        strcat(message, " ");

    snprintf(message + strlen(message), message_size - strlen(message) - 1,
             expected_value_name,
             constraint->expected_value_name);

    if (actual_value_not_necessary_for(constraint, actual_string, actual_value_string)) {
        /* when the actual string and the actual value are the same, don't print both of them */
        /* also, don't print "0" for false and "1" for true */
        /* also, don't print expected/actual for contents constraints since that is useless */
        return message;
    }

    /* for string constraints, print out the strings encountered and not their pointer values */
    if (values_are_strings_in(constraint)) {
        snprintf(message + strlen(message), message_size - strlen(message) - 1,
                 actual_value_as_string,
                 (const char *)actual);
        if (strstr(constraint->name, "not ") == NULL || strstr(constraint->name, "contain ") != NULL) {
            strcat(message, "\n");
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                     constraint->expected_value_message,
                     (const char *)constraint->expected_value);
        }
        /* The final string may have percent characters, so, since it is
           later used in a (v)printf, we have to double them
        */
        double_all_percent_signs_in(&message);
        return message;
    }

    if (is_content_comparing(constraint)) {
        int difference_index = find_index_of_difference((char *)constraint->expected_value,
                                                        (char *)actual, constraint->size_of_expected_value);
        snprintf(message + strlen(message), message_size - strlen(message) - 1,
                 at_offset,
                 difference_index);
        return message;
    }

    snprintf(message + strlen(message), message_size - strlen(message) - 1,
             actual_value_message,
             actual);

    if (strstr(constraint->name, "not ") == NULL) {
        strcat(message, "\n");
        snprintf(message + strlen(message), message_size - strlen(message) - 1,
                 constraint->expected_value_message,
                 constraint->expected_value);
    }

    return message;
}

#ifdef __cplusplus
}
#endif
