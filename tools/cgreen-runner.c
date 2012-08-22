#include <cgreen/reporter.h>
#include <cgreen/runner.h>
#include <cgreen/suite.h>
#include <cgreen/text_reporter.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

struct test_item {
    char *name;
};

#define cgreen_spec_prefix "CgreenSpec_"
#define cgreen_suite_default "default_"

#if defined(__CYGWIN__) || defined(__APPLE__)
// Cygwin and MacOSX nm lists external names with a leading '_'
// which dlsym() doesn't want, so we'll include the '_' in the separator
#  define nm_output_column_separator " _"
#else
#  define nm_output_column_separator " "
#endif



static uint32_t discover_tests_in(const char *, struct test_item *, const uint32_t);
static void add_discovered_tests_to_suite(void *handle, struct test_item* tests, const uint32_t number_of_tests, TestSuite* suite);
static int file_exists (const char *filename);
static void reflective_runner_cleanup(void*);

int main(int argc, char **argv) {
    int status;
    void *test_library_handle;

    /* TODO: use getopt() for better command line parsing. */
    /* TODO: use command line option -v (verbose) to reduce printf output for regular program execution. */
    /* TODO: show usage help for argc<2 and option -h (help) */
    /* TODO: in addition to running only a single named test, match tests with a substring and run only those */
    if (argc < 2) {
        printf("Usage: cgreen-runner <test library filename> [test name]\n");
        exit(1);
    }

    const char* test_library = argv[1];
    const char* test_name = NULL;
    if (argc == 3) {
	test_name = argv[2];
    }

    if (!file_exists(test_library)) {
        printf("Couldn't find library: %s\n", test_library);
        exit(1);
    }

    const uint32_t MAXIMUM_NUMBER_OF_TESTS = 2048;
    struct test_item discovered_tests[MAXIMUM_NUMBER_OF_TESTS];
    memset(discovered_tests, 0, sizeof(discovered_tests));
    const uint32_t number_of_tests = discover_tests_in(test_library, discovered_tests, MAXIMUM_NUMBER_OF_TESTS);

    printf("Discovered: %d tests\n", number_of_tests);

    printf("Opening [%s]\n", test_library);
    test_library_handle = dlopen (test_library, RTLD_NOW);
    if (test_library_handle == NULL) {
        fprintf (stderr, "dlopen failure (error: %s)\n", dlerror());
        exit(1);
    }

    TestSuite *suite = create_test_suite();
    TestReporter *reporter = create_text_reporter();

    add_discovered_tests_to_suite(test_library_handle, discovered_tests, number_of_tests, suite);

    if (test_name) {
	char *test_name_with_prefix = malloc(strlen(cgreen_spec_prefix) +
                                             strlen(cgreen_suite_default) +
                                             strlen(test_name) + 1);
	strcpy(test_name_with_prefix, cgreen_spec_prefix);
	strcat(test_name_with_prefix, cgreen_suite_default);
	strcat(test_name_with_prefix, test_name);
	printf("Only running test %s ...\n", test_name);
	status = run_single_test(suite, test_name_with_prefix, reporter);
	free(test_name_with_prefix);
    } else {
	status = run_test_suite(suite, reporter);
    }

    reflective_runner_cleanup(test_library_handle);

    for(uint32_t i = 0; i != number_of_tests; ++i)
	free(discovered_tests[i].name);

    return status;
}

static int file_exists(const char *filename)
{
    return (access(filename, F_OK) == 0);
}

static void reflective_runner_cleanup(void *handle)
{
    dlclose(handle);
}

// XXX: hack to use nm command-line utility for now.  Use libelf later.
static uint32_t discover_tests_in(const char* test_library, struct test_item* test_items, const uint32_t maximum_number_of_test_items)
{
    char cmd[2048];
    strcpy(cmd, "/usr/bin/nm ");
    strcat(cmd, test_library);

    /* Open the command for reading. */
    FILE *nm_output_pipe = popen(cmd, "r");
    if (nm_output_pipe == NULL) {
        printf("Failed to run command\n" );
        return 0;
    }

    uint32_t number_of_tests = 0;
    char line[1024];
    while (fgets(line, sizeof(line)-1, nm_output_pipe) != NULL) {
        char *match = strstr(line, nm_output_column_separator cgreen_spec_prefix);
        if (match != NULL) {
            match += strlen(nm_output_column_separator);

            if (0 == strncmp(match, cgreen_spec_prefix, strlen(cgreen_spec_prefix)))
                {
                    match[strlen(match) - 1] = 0; /* remove newline */
                    test_items[number_of_tests].name = strdup(match);

                    // const size_t cgreen_spec_prefix_length = strlen(cgreen_spec_prefix);
                    // printf("Discovered test %s ...\n", match + cgreen_spec_prefix_length);

                    if (++number_of_tests > maximum_number_of_test_items) {
                        printf("Found too many tests (%d)! Giving up. Consider splitting tests between libraries on logical suite boundaries.\n", number_of_tests);
                        exit(1);
                    }
                }
        }
    }

    pclose(nm_output_pipe);

    return number_of_tests;
}

static void add_discovered_tests_to_suite(void *handle, struct test_item* tests, const uint32_t number_of_tests, TestSuite* suite)
{
    for (uint32_t i = 0; i < number_of_tests; i++) {
        //printf("Discovered test %s ...\n", testlist[i].name);

        char *error;
        CgreenTest *cgreen_test = (CgreenTest *)(dlsym(handle, tests[i].name));

        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }

        add_test_(suite, tests[i].name, cgreen_test);
    }
}
