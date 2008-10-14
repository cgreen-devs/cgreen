#include <cgreen/cgreen.h>
#include <cgreen/slurp.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void assert_slurped(char *path, int gulp, const char *expected_contents);

Ensure missing_file_gives_null() {
    assert_equal(slurp("samples/not_there", 1024), NULL);
}

Ensure whole_file_can_be_read() {
    assert_slurped("samples/some_file", 1024, "Some stuff\n");
}

Ensure whole_file_can_be_read_in_multiple_small_blocks() {
    assert_slurped("samples/some_file", 1, "Some stuff\n");
}

static void assert_slurped(char *path, int gulp, const char *expected_contents) {
    char *full_path = malloc(256 + strlen(path) + 2);
    getcwd(full_path, 256);
    strcpy(full_path + strlen(full_path), "/");
    strcpy(full_path + strlen(full_path), path);
    char *buffer = slurp(full_path, gulp);
    free(full_path);
    assert_string_equal(buffer, expected_contents);
    free(buffer);
}

TestSuite *slurp_tests() {
    TestSuite *suite = create_test_suite();
    add_test(suite, missing_file_gives_null);
    add_test(suite, whole_file_can_be_read);
    add_test(suite, whole_file_can_be_read_in_multiple_small_blocks);
    return suite;
}
