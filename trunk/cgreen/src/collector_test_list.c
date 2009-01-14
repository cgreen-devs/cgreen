#include <cgreen/collector_test_list.h>
#include <cgreen/vector.h>
#include <stdio.h>
#include <stdlib.h>

static CgreenVector *tests;

void destroy_string(void *string) {
    free((char *)string);
}

void create_test_list() {
    tests = create_cgreen_vector(&destroy_string);
}

void destroy_test_list() {
    destroy_cgreen_vector(tests);
}

void add_to_test_list(char *test_name) {
    cgreen_vector_add(tests, test_name);
}

void reset_test_list() {
    destroy_cgreen_vector(tests);
    tests = create_cgreen_vector(&destroy_string);
}

void print_test_list(FILE *file) {
    int i;
    for (i = 0; i < cgreen_vector_size(tests); i++) {
        fprintf(file, "%s&%s", (i == 0 ? "" : ", "), (char *) cgreen_vector_get(tests, i));
    }
}
