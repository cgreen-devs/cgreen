#include <cgreen/parameters.h>
#include <cgreen/vector.h>
#include <stdlib.h>
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
static char *strip_box_double(char *token);
static char *strip_d_macro(char *token);

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
        token = strip_d_macro(strip_box_double(skip_nulls_until(token, parameters_end)));
        cgreen_vector_add(names, (void*)stringdup(token));
        token = end_of_token(token);
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

static char *strip_box_double(char *token) {
    if ((strncmp("box_double(", token, 11) == 0) && (*(end_of_token(token) - 1) == ')')) {
        memmove(token, token + 11, strlen(token) - 11 + 1);
        *(end_of_token(token) - 1) = '\0';
    }

    return token;
}

static char *strip_d_macro(char *token) {
    if ((strncmp("d(", token, 2) == 0) && (*(end_of_token(token) - 1) == ')')) {
        memmove(token, token + 2, strlen(token) - 2 + 1);
        *(end_of_token(token) - 1) = '\0';
    }

    return token;
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
