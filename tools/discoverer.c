#include "discoverer.h"

#include "io.h"

CgreenVector *discover_tests_in(const char *filename) {
    FILE *library = open_file(filename, "r");
    if (library == NULL)
        return NULL;
    else {
        FILE *nm_output_pipe = open_process("nm test", "r");
        if (nm_output_pipe != NULL) {
            char buffer[100];
            CgreenVector *tests = create_cgreen_vector(NULL);
            int length = read_line(nm_output_pipe, buffer, sizeof(buffer)-1);
            if (length > -1) {
                cgreen_vector_add(tests, NULL);
                read_line(nm_output_pipe, buffer, sizeof(buffer)-1);
            }
            return tests;
        } else
            return NULL;
    }
}
