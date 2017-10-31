#include "discoverer.h"

#include "io.h"

#include <string.h>
#include <stdbool.h>

#include <cgreen/internal/unit_implementation.h>


static bool contains_cgreen_spec(const char *line) {
    return strstr(line, CGREEN_SPEC_PREFIX CGREEN_SEPARATOR) != NULL;
}

static bool is_definition(const char *line) {
    return strstr(line, " D ") != NULL;
}

static void add_all_tests_from(FILE *nm_output_pipe, CgreenVector *tests) {
    char line[100];
    int length = read_line(nm_output_pipe, line, sizeof(line)-1);

    while (length > -1) {
        if (contains_cgreen_spec(line) && is_definition(line))
            cgreen_vector_add(tests, NULL); /* TODO Should add test structs */
        length = read_line(nm_output_pipe, line, sizeof(line)-1);
    }
}

CgreenVector *discover_tests_in(const char *filename) {
    FILE *library = open_file(filename, "r");
    if (library == NULL)
        return NULL;
    else {
        FILE *nm_output_pipe = open_process("nm test", "r");
        if (nm_output_pipe != NULL) {
            CgreenVector *tests = create_cgreen_vector(NULL);
            add_all_tests_from(nm_output_pipe, tests);
            return tests;
        } else
            return NULL;
    }
}
