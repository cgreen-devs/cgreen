#include <cgreen/cgreen.h>
#include "xml_reporter.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "runner.h"


typedef struct test_item {
    char *symbol;
} TestItem;

#define CGREEN_DEFAULT_SUITE "default"


/*----------------------------------------------------------------------*/
static char *mangle_test_name(const char *original_test_name) {

    char *context = strdup(original_test_name);
    const char *test_name = strchr(original_test_name, ':')+1;
    if (strchr(context, ':') != NULL) {
        *strchr(context, ':') = '\0';
    } else {
        free(context);
        context = strdup(CGREEN_DEFAULT_SUITE);
        test_name = original_test_name;
    }
    
    char *test_name_with_prefixes = (char *)malloc(strlen(CGREEN_SPEC_PREFIX) +
                                                   strlen(CGREEN_SEPARATOR) +
                                                   strlen(context) +
                                                   strlen(CGREEN_SEPARATOR) +
                                                   strlen(test_name) + 1);
    strcpy(test_name_with_prefixes, CGREEN_SPEC_PREFIX);
    strcat(test_name_with_prefixes, CGREEN_SEPARATOR);
    strcat(test_name_with_prefixes, context);
    strcat(test_name_with_prefixes, CGREEN_SEPARATOR);
    strcat(test_name_with_prefixes, test_name);
    return test_name_with_prefixes;
}


/*----------------------------------------------------------------------*/
static void add_discovered_tests_to_suite(void *handle, TestItem *tests, TestSuite *suite)
{
    for (int i = 0; tests[i].symbol != NULL; i++) {
        char *error;
        CgreenTest *cgreen_test = (CgreenTest *)(dlsym(handle, tests[i].symbol));

        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }

        add_test_(suite, tests[i].symbol, cgreen_test);
    }
}


/*----------------------------------------------------------------------*/
static bool ensure_test_exists(char *test_name_with_prefix, TestItem discovered_tests[]) {
    for (int i = 0; discovered_tests[i].symbol != NULL; i++)
        if (strcmp(discovered_tests[i].symbol, test_name_with_prefix) == 0) {
            return true;
        }
    return false;
}


/*----------------------------------------------------------------------*/
static void reflective_runner_cleanup(void *handle)
{
    dlclose(handle);
}


/*----------------------------------------------------------------------*/
static int run_tests(TestReporter *reporter, const char *test_name, void *test_library_handle, TestItem discovered_tests[], int number_of_tests, bool verbose) {
    int status;
    TestSuite *suite = create_named_test_suite("main");

    add_discovered_tests_to_suite(test_library_handle, discovered_tests, suite);

    if (test_name) {
        char *test_name_with_prefix = mangle_test_name(test_name);
        bool found = ensure_test_exists(test_name_with_prefix, discovered_tests);
        if (verbose)
            printf(" to only run test '%s' ...\n", test_name);
        if (!found) {
            fprintf(stderr, "ERROR: No such test: '%s'\n", test_name);
            exit(1);
        }
        status = run_single_test(suite, test_name_with_prefix, reporter);
        free(test_name_with_prefix);
    } else {
        if (verbose)
            printf(" to run all discovered tests ...\n");
        status = run_test_suite(suite, reporter);
    }

    reflective_runner_cleanup(test_library_handle);

    for (int i = 0; i != number_of_tests; ++i)
        free(discovered_tests[i].symbol);
    return(status);
}


/*----------------------------------------------------------------------*/
static void register_test(TestItem *test_items, int maximum_number_of_tests, char *symbol) {
    int number_of_tests;

    for (number_of_tests = 0; test_items[number_of_tests].symbol != NULL; number_of_tests++);
    test_items[number_of_tests].symbol = strdup(symbol);

    if (number_of_tests == maximum_number_of_tests) {
        fprintf(stderr, "\nERROR: Found too many tests (%d)! Giving up.\nConsider splitting tests between libraries on logical suite boundaries.\n", number_of_tests);
        exit(1);
    }
}


/*----------------------------------------------------------------------*/
static const char *start_of_context_name(const char *name) {
    return strstr(name, CGREEN_SPEC_PREFIX)+strlen(CGREEN_SPEC_PREFIX)+strlen(CGREEN_SEPARATOR);
}


/*----------------------------------------------------------------------*/
static const char *test_name(const char *name) {
    return strstr(start_of_context_name(name), CGREEN_SEPARATOR) + strlen(CGREEN_SEPARATOR);
}


/*----------------------------------------------------------------------*/
static const char *suite_name(const char *name) {
    static char copy[1000];
    strcpy(copy, start_of_context_name(name));
    *strstr(copy, CGREEN_SEPARATOR) = '\0';
    return copy;
}


/*----------------------------------------------------------------------*/
static bool is_cgreen_spec(const char* line) {
    return strstr(line, CGREEN_SPEC_PREFIX) != NULL;
}


#if defined(__CYGWIN__) || defined(__APPLE__)
// Cygwin and MacOSX nm lists external names with a leading '_'
// which dlsym() doesn't want, so we'll include the '_' in the separator
#  define NM_OUTPUT_COLUMN_SEPARATOR "D _"
#else
#  define NM_OUTPUT_COLUMN_SEPARATOR "D "
#endif

/*----------------------------------------------------------------------*/
// XXX: hack to use nm command-line utility for now.  Use libelf later.
// XXX: but nm is more portable across object formats...
static uint32_t discover_tests_in(const char* test_library, TestItem* test_items, const uint32_t maximum_number_of_test_items, bool verbose)
{
    char cmd[2048];
    strcpy(cmd, "/usr/bin/nm ");
    strcat(cmd, test_library);

    /* Open the command for reading. */
    FILE *nm_output_pipe = popen(cmd, "r");
    if (nm_output_pipe == NULL) {
        printf("\nERROR: Failed to run command ('/usr/bin/nm')\n" );
        return 0;
    }

    uint32_t number_of_tests = 0;
    char line[1024];
    while (fgets(line, sizeof(line)-1, nm_output_pipe) != NULL) {
        if (is_cgreen_spec(line)) {
            char *symbol = strstr(line, NM_OUTPUT_COLUMN_SEPARATOR) + strlen(NM_OUTPUT_COLUMN_SEPARATOR);
            symbol[strlen(symbol) - 1] = 0; /* remove newline */
            if (verbose)
                printf("Discovered %s:%s\n", suite_name(symbol), test_name(symbol));
            register_test(test_items, maximum_number_of_test_items, symbol);
            number_of_tests++;
        }
    }

    pclose(nm_output_pipe);

    return number_of_tests;
}


/*======================================================================*/
int runner(TestReporter *reporter, const char *test_library, const char *test_name, bool verbose, bool no_run) {
    int status = 0;
    void *test_library_handle;
    const uint32_t MAXIMUM_NUMBER_OF_TESTS = 2048;
    TestItem discovered_tests[MAXIMUM_NUMBER_OF_TESTS];
    memset(discovered_tests, 0, sizeof(discovered_tests));

    const uint32_t number_of_tests = discover_tests_in(test_library, discovered_tests, MAXIMUM_NUMBER_OF_TESTS, verbose);

    if (verbose)
        printf("Discovered %d tests\n", number_of_tests);

    if (!no_run) {
        if (verbose)
            printf("Opening [%s]", test_library);
        test_library_handle = dlopen (test_library, RTLD_NOW);
        if (test_library_handle == NULL) {
            fprintf (stderr, "\nERROR: dlopen failure (error: %s)\n", dlerror());
            exit(1);
        }
	status = run_tests(reporter, test_name, test_library_handle, discovered_tests, number_of_tests, verbose);
    }
    return(status);
}
