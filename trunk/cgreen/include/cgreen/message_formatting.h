#ifndef CGREEN_MESSAGE_FORMATTING_H
#define CGREEN_MESSAGE_FORMATTING_H

#include <cgreen/constraint.h>
#include <stdint.h>

#ifdef __cplusplus
namespace cgreen {
extern "C" {
#endif

void format_expectation_failure_message_for(char *message, size_t message_size, Constraint *constraint, const char *actual_string, intptr_t actual);
void format_validation_failure_message_for(char *message, size_t message_size, const char *function, Constraint *constraint, intptr_t actual);
bool parameters_are_not_valid_for(Constraint *constraint, intptr_t actual);
#ifdef __cplusplus
}
}
#endif

#endif
