#include <cgreen/parameters.h>
#include <cgreen/vector.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifdef __cplusplus
namespace cgreen {
#endif
#ifdef _MSC_VER
//disable warning on windows
//'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup.
#pragma warning(disable:4996)
#endif

static char *tokenise_by_commas_and_whitespace(char *list);
static char *skip_nulls_until(char *pointer, char *pointer_end);
static char *end_of_token(char *token);
static char *strip_function_from(char *token, const char *function_name);

static char *stringdup(const char *string) {
    return strcpy((char *)malloc(strlen(string)+1), string);
}

CgreenVector *create_vector_of_names(const char *parameters) {
    char *parameters_to_tokenize;
    char *parameters_end;
    char *tokens;
    char *token;
    CgreenVector *names = create_cgreen_vector(&free);
    if ((parameters == NULL) || (strlen(parameters) == 0)) {
        return names;
    }

    parameters_to_tokenize = stringdup(parameters);
    if (parameters_to_tokenize == NULL) {
        return names;
    }

    parameters_end = parameters_to_tokenize + strlen(parameters_to_tokenize);
    tokens = tokenise_by_commas_and_whitespace(parameters_to_tokenize);
    token = tokens;
    while (token < tokens + strlen(parameters)) {
        token = skip_nulls_until(token, parameters_end);
        int length_of_token = strlen(token);
        token = strip_function_from(token, "box_double");
        token = strip_function_from(token, "d");
        cgreen_vector_add(names, (void*)stringdup(token));
        token += length_of_token;
    }

    free(tokens);
    return names;
}

static char *tokenise_by_commas_and_whitespace(char *list) {
    size_t i, length;

    for (i = 0, length = strlen(list); i < length; i++) {
        if (isspace((int)list[i]) || list[i] == ',') {
            list[i] = '\0';
        }
    }

    return list;
}

static char *skip_nulls_until(char *pointer, char *pointer_end) {
    while (*pointer == '\0' && pointer < pointer_end) {
        pointer++;
    }

    return pointer;
}

static char *end_of_token(char *token) {
    return token + strlen(token);
}

static char last_char_of(char *token) {
    return *(end_of_token(token) - 1);
}

static bool begins_with(char *token, const char *beginning) {
    return strncmp(beginning, token, strlen(beginning)) == 0;
}

static void move_parameter_to_beginning_of(char *token, const char *function_name, int parameter_length) {
    memmove(token, token + strlen(function_name) + strlen("("), parameter_length+strlen(")")+1);
    *(end_of_token(token) - 1) = '\0';
}

static char *strip_function_from(char *token, const char *function_name) {
    if (begins_with(token, function_name) && token[strlen(function_name)] == '(' && last_char_of(token) == ')') {
        int parameter_length = strlen(token) - strlen(function_name) - strlen("()");
	move_parameter_to_beginning_of(token, function_name, parameter_length);
    }

    return token;
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
