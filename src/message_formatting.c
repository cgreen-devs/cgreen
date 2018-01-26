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

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

#ifdef _MSC_VER
#include "wincompat.h"
#endif

#include "constraint_internal.h"


// Handling of percent signs
static const char *next_percent_sign(const char *s) {
    return strchr(s, '%');
}

static size_t count_percent_signs(char const *s)
{
    size_t count = 0;
    char const *p = next_percent_sign(s);
    while (p != NULL) {
        count++;
        p = next_percent_sign(p+1);
    }
    return count;
}


static char *copy_while_doubling_percent_signs(char *string, char const *original)
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


static char *double_all_percent_signs_in(const char *original)
{
    size_t percent_count = count_percent_signs(original);
    char *new_string = (char *)malloc(strlen(original) + percent_count + 1);
    if (new_string == NULL) {
        return NULL;
    }
    copy_while_doubling_percent_signs(new_string, original);
    return new_string;
}


static int find_index_of_difference(void *expected, void *actual, size_t size_to_compare) {
    char *expectedp = expected;
    char *actualp = actual;

    while (size_to_compare--) {
        if (*expectedp++ != *actualp++) {
            return (int)((void *)actualp - actual)-1;
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
        "Wanted to compare contents with [%s],\n"
        "\t\tbut [%ld] was given for the comparison size.";
    const char *null_used_for_compare_message =
        "Wanted to compare contents with [%s],\n"
        "\t\tbut NULL was used for the pointer we wanted to compare to.\n"
        "\t\tIf you want to explicitly check for null, use the is_null constraint instead.";
    const char *null_used_for_actual_message =
        "Wanted to compare contents of [%s] but it had a value of NULL.\n"
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
        const char *as_string;
        if (constraint->parameter_name != NULL) {
            as_string = constraint->parameter_name;
        } else {
            as_string = constraint->expected_value_name;
        }

        if ((long signed)constraint->size_of_expected_value <= 0) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                    name_has_incorrect_size_message,
                    as_string,
                    (long signed)constraint->size_of_expected_value);

            return message;
        }

        if ((void *)actual == NULL) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                    null_used_for_compare_message,
                    as_string);

                return message;
        }

        if (constraint->expected_value.value.pointer_value == NULL) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                    null_used_for_actual_message,
                    as_string);

                return message;
        }
    }

    return message;
}


static bool is_not_equal_to_string_constraint(Constraint *constraint) {
    return strstr(constraint->name, "not ") != NULL && strstr(constraint->name, "equal ") != NULL;
}


char *failure_message_for(Constraint *constraint, const char *actual_string, intptr_t actual_value) {
    char actual_int_value_string[32];
    const char *constraint_as_string_format = "Expected [%s] to [%s]";
    const char *expected_value_string_format =  "[%s]";
    const char *actual_value_string_format = "\n\t\tactual value:\t\t\t[\"%s\"]";
    const char *at_offset = "\n\t\tat offset:\t\t\t[%d]";
    const char *expected_content = "\n\t\t\tactual value:\t\t[0x%02x]\n\t\t\texpected value:\t\t[0x%02x]";
    const char *actual_value_as_string;
    char *message;
    size_t message_size = strlen(constraint_as_string_format) +
            strlen(expected_value_string_format) +
            strlen(actual_value_string_format) +
            strlen(at_offset) +
            strlen(constraint->actual_value_message) +
            strlen(constraint->expected_value_message) +
            strlen(constraint->expected_value_name) +
            strlen(constraint->name) +
            strlen(actual_string) +
            512; // just in case

    snprintf(actual_int_value_string, sizeof(actual_int_value_string) - 1, "%" PRIdPTR, actual_value);

    if (values_are_strings_in(constraint)) {
        message_size += strlen(constraint->expected_value.value.string_value);
        if (actual_value != (intptr_t)NULL) {
            message_size += strlen((char *)actual_value);
        }
    }

    message = (char *)malloc(message_size);

    /* if the actual value expression contains '%' we want it to survive the final expansion with
       arguments that happens in assert_true() */
    actual_value_as_string = double_all_percent_signs_in(actual_string);

    /* expand the constraint with the actual value in string format... */
    snprintf(message, message_size - 1,
             constraint_as_string_format,
             actual_value_as_string,
             constraint->name);

    free((void*)actual_value_as_string);

    if (no_expected_value_in(constraint)) {
        return message;
    } else
        strcat(message, " ");

    /* expand the expected value string for all assertions that have one... */
    snprintf(message + strlen(message), message_size - strlen(message) - 1,
             expected_value_string_format,
             constraint->expected_value_name);

    if (actual_value_not_necessary_for(constraint, actual_string, actual_int_value_string)) {
        /* when the actual string and the actual value are the same, don't print both of them */
        /* also, don't print "0" for false and "1" for true */
        /* also, don't print expected/actual for contents constraints since that is useless */
        return message;
    }

    /* for string constraints, print out the strings encountered and not their pointer values */
    if (values_are_strings_in(constraint)) {
        snprintf(message + strlen(message), message_size - strlen(message) - 1,
                 actual_value_string_format,
                 (const char *)actual_value);
        if (!is_not_equal_to_string_constraint(constraint)) {
            strcat(message, "\n");
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                     constraint->expected_value_message,
                     constraint->expected_value.value.string_value);
        }
        /* The final string may have percent characters, so, since it is
           later used in a (v)printf, we have to double them
        */
        if (next_percent_sign(message) != NULL) {
            char *message_with_doubled_percent_signs = double_all_percent_signs_in(message);
            free(message);
            message = message_with_doubled_percent_signs;
        }
        return message;
    }

    /* show difference for contents as a position */
    if (is_content_comparing(constraint)) {
        int difference_index = find_index_of_difference(constraint->expected_value.value.pointer_value,
                                                        (void *)actual_value,
                                                        constraint->size_of_expected_value);
        if (difference_index != -1) {
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                     at_offset,
                     difference_index);
            snprintf(message + strlen(message), message_size - strlen(message) - 1,
                     expected_content,
                     ((char *)actual_value)[difference_index],
                     ((char *)constraint->expected_value.value.pointer_value)[difference_index]);
        }
        return message;
    }

    /* add the actual value */
    snprintf(message + strlen(message), message_size - strlen(message) - 1,
             constraint->actual_value_message,
             actual_value);

    /* add the expected value */
    if (strstr(constraint->name, "not ") == NULL) {
        strcat(message, "\n");
        snprintf(message + strlen(message), message_size - strlen(message) - 1,
                 constraint->expected_value_message,
                 constraint->expected_value.value.string_value);
    }

    return message;
}
