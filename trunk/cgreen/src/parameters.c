#include <cgreen/parameters.h>
#include <cgreen/vector.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *tokenise_by_commas_and_whitespace(char *list);
static char *skip_nulls(char *pointer);
static char *end_of_token(char *token);
static char *strip_box_double(char *token);
static char *strip_d_macro(char *token);

CgreenVector *create_vector_of_names(const char *parameters) {
    CgreenVector *names = create_cgreen_vector(&free);
    if ((parameters == NULL) || (strlen(parameters) == 0)) {
        return names;
    }
    char *tokens = tokenise_by_commas_and_whitespace(strdup(parameters));
    char *token = tokens;
    while (token < tokens + strlen(parameters)) {
        token = strip_d_macro(strip_box_double(skip_nulls(token)));
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

/* vim: set ts=4 sw=4 et cindent: */
