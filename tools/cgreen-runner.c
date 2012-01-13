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
    char name[512];
};


static uint32_t discover_tests_in(const char *, struct test_item *, uint32_t);
static void add_discovered_tests_to_suite(void *handle, struct test_item* tests, uint32_t number_of_tests, TestSuite* suite);
static int file_exists (const char *filename);
static void reflective_runner_cleanup(void*);

int main(int argc, char **argv) {
    int status;
    void *test_library_handle;

    if (argc < 2) {
        printf("Usage: cgreen-runner <test library filename>\n");
        exit(1);
    }

    const char* test_library = argv[argc - 1];

    if (!file_exists(test_library)) {
        printf("Couldn't find library: %s\n", test_library);
        exit(1);
    }

    const uint32_t MAXIMUM_NUMBER_OF_TESTS = 2048;
    struct test_item discovered_tests[MAXIMUM_NUMBER_OF_TESTS];
    memset(discovered_tests, 0, sizeof(discovered_tests));
    uint32_t number_of_tests = discover_tests_in(test_library, discovered_tests, MAXIMUM_NUMBER_OF_TESTS);

    printf("Discovered: %d tests\n", number_of_tests);

    printf("Opening [%s]\n", test_library);
    test_library_handle = dlopen (test_library, RTLD_LAZY);
    if (test_library_handle == NULL) {
        fprintf (stderr, "dlopen failure (error: %s)\n", dlerror());
        exit(1);
    }

    TestSuite *suite = create_test_suite();
    TestReporter *reporter = create_text_reporter();

    add_discovered_tests_to_suite(test_library_handle, discovered_tests, number_of_tests, suite);

    status = run_test_suite(suite, reporter);

    reflective_runner_cleanup(test_library_handle);

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
static uint32_t discover_tests_in(const char* test_library, struct test_item* test_items, uint32_t maximum_number_of_test_items)
{
    char cmd[2048];

    memset(cmd, 0, sizeof(cmd));
    strcat(cmd, "/usr/bin/nm ");
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
       char *match = strstr(line, "CgreenSpec");

       if (match != NULL) {
           size_t match_string_length = strlen(match);

           if (0 != strncmp(match, "CgreenSpec_default", strlen("CgreenSpec_default")))
           {
               strncpy(test_items[number_of_tests].name, match, match_string_length - 1);
               number_of_tests++;
           }
       }

       if (number_of_tests > maximum_number_of_test_items) {
    	   printf("Found too many tests (%d)! Giving up. Consider splitting tests between libraries on logical suite boundaries.\n", number_of_tests);
    	   exit(1);
       }
    }

    pclose(nm_output_pipe);

    return number_of_tests;
}

static void add_discovered_tests_to_suite(void *handle, struct test_item* tests, uint32_t number_of_tests, TestSuite* suite)
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
