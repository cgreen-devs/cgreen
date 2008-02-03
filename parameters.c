#include "parameters.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *tokenise_by_commas_and_whitespace(char *list);
static char *skip_nulls(char *pointer);
static char *end_of_token(char *token);

CgreenVector *create_vector_of_names(const char *parameters) {
    CgreenVector *names = create_cgreen_vector(&free);
    if ((parameters == NULL) || (strlen(parameters) == 0)) {
        return names;
    }
    char *tokens = tokenise_by_commas_and_whitespace(strdup(parameters));
    char *token = tokens;
    while (token < tokens + strlen(parameters)) {
        token = skip_nulls(token);
        cgreen_vector_add(names, strdup(token));
        token = end_of_token(token);
    }
    free(tokens);
    return names;
}

static char *tokenise_by_commas_and_whitespace(char *list) {
    int i, length;
    for (i = 0, length = strlen(list); i < length; i++) {
        if (isspace(list[i]) || list[i] == ',') {
            list[i] = '\0';
        }
    }
    return list;
}

static char *skip_nulls(char *pointer) {
    while (*pointer == '\0') {
        pointer++;
    }
    return pointer;
}

static char *end_of_token(char *token) {
    return token + strlen(token);
}
