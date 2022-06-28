#include "discoverer.h"

#include "io.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <cgreen/internal/unit_implementation.h>

#include "../src/utils.h"


#include "test_item.h"

static const char *cgreen_spec_start_of(const char *line) {
    return strstr(line, CGREEN_SPEC_PREFIX CGREEN_SEPARATOR);
}

static bool contains_cgreen_spec(const char *line) {
    return cgreen_spec_start_of(line) != NULL;
}

static bool is_definition(const char *line) {
    return strstr(line, " D ") != NULL;
}

static bool complete_line_read(char line[]) {
    return line[strlen(line)-1] == '\n';
}


static void strip_newline_from(char *name) {
    if (name[strlen(name)-1] == '\n')
        name[strlen(name)-1] = '\0';
}

static void add_all_tests_from(FILE *nm_output_pipe, CgreenVector *tests, bool verbose) {
    char line[1000];
    int length = read_line(nm_output_pipe, line, sizeof(line)-1);
    while (length > -1) {       /* TODO: >0 ? */
        if (!complete_line_read(line))
            PANIC("Too long line in nm output");
        if (contains_cgreen_spec(line) && is_definition(line)) {
            strip_newline_from(line);
            TestItem *test_item = create_test_item_from(cgreen_spec_start_of(line));
            if (verbose)
                printf("Discovered %s:%s (%s)\n", test_item->context_name, test_item->test_name,
                       test_item->specification_name);
            cgreen_vector_add(tests, test_item);
        }
        length = read_line(nm_output_pipe, line, sizeof(line)-1);
    }
}

CgreenVector *discover_tests_in(const char *filename, bool verbose) {
    FILE *library = open_file(filename, "r");
    if (library == NULL)
        return NULL;
    close_file(library);

    char nm_command[1000];
    sprintf(nm_command, "/usr/bin/nm '%s'", filename);
    FILE *nm_output_pipe = open_process(nm_command, "r");
    if (nm_output_pipe == NULL)
        return NULL;

    CgreenVector *tests = create_cgreen_vector((GenericDestructor)&destroy_test_item);
    add_all_tests_from(nm_output_pipe, tests, verbose);
    close_process(nm_output_pipe);
    return tests;
}
