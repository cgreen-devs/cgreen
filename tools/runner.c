#include <cgreen/cgreen.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fnmatch.h>

#include "xml_reporter.h"
#include "runner.h"


typedef struct test_item {
    char *symbol;
    const char *context;
    const char *test_name;
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
static bool is_cgreen_spec(const char* line) {
    return strstr(line, CGREEN_SPEC_PREFIX) != NULL;
}


/*----------------------------------------------------------------------*/
static char *context_name_of(const char* name) {
    char *context = strdup(name);
    char *colon = strchr(context, ':');
    if (colon)
        *colon = '\0';
    else
        return CGREEN_DEFAULT_SUITE;
    return context;
}


/*----------------------------------------------------------------------*/
static char *test_name_of(const char *name) {
    char *test = strdup(name);
    char *colon = strchr(test, ':');
    if (colon)
        return colon+1;
    else
        return test;
}


/*----------------------------------------------------------------------*/
static bool test_name_matches(const char *test_name, TestItem test) {
    //    return strcmp(test.context, context_name_of(test_name)) == 0 &&
    //        strcmp(test.test_name, test_name_of(test_name)) == 0;
    return fnmatch(context_name_of(test_name), test.context, 0) == 0 &&
        fnmatch(test_name_of(test_name), test.test_name, 0) == 0;
}


/*----------------------------------------------------------------------*/
static int add_matching_tests_to_suite(void *handle, const char *test_name_pattern, TestItem *test_items, TestSuite *suite)
{
    int count = 0;

    for (int i = 0; test_items[i].symbol != NULL; i++) {
        if (test_name_pattern == NULL || test_name_matches(test_name_pattern, test_items[i])) {
            char *error;
            CgreenTest *cgreen_test = (CgreenTest *)(dlsym(handle, test_items[i].symbol));

            if ((error = dlerror()) != NULL)  {
                fprintf (stderr, "%s\n", error);
                exit(1);
            }

            add_test_(suite, test_items[i].symbol, cgreen_test);
            count++;
        }
    }
    return count;
}


/*----------------------------------------------------------------------*/
static const char *start_of_context_name(const char *symbol) {
    char *context = strdup(symbol);
    return strstr(context, CGREEN_SPEC_PREFIX)+strlen(CGREEN_SPEC_PREFIX)+strlen(CGREEN_SEPARATOR);
}


/*----------------------------------------------------------------------*/
static const char *test_name_of_symbol(const char *symbol) {
    char *test_name = strdup(symbol);
    return strstr(start_of_context_name(test_name), CGREEN_SEPARATOR) + strlen(CGREEN_SEPARATOR);
}


/*----------------------------------------------------------------------*/
static const char *suite_name_of_symbol(const char *symbol) {
	char *copy = strdup(start_of_context_name(symbol));
    *strstr(copy, CGREEN_SEPARATOR) = '\0';
    return copy;
}


/*----------------------------------------------------------------------*/
static bool matching_test_exists(const char *test_name, TestItem tests[]) {
    for (int i = 0; tests[i].symbol != NULL; i++)
        if (test_name_matches(test_name, tests[i])) {
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
static int count(TestItem test_items[]) {
    int i;
    for (i = 0; test_items[i].symbol != NULL; i++);
    return i;
}


/*----------------------------------------------------------------------*/
static int run_tests(TestReporter *reporter, const char *test_name, void *test_library_handle, TestItem test_items[], bool verbose) {
    int status;
    TestSuite *suite = create_named_test_suite("main");

    int number_of_matches = add_matching_tests_to_suite(test_library_handle, test_name, test_items, suite);

    if (test_name != NULL && number_of_matches == 1) {
        char *test_name_as_symbol = mangle_test_name(test_name);
        bool found = matching_test_exists(test_name, test_items);
        if (verbose)
            printf(" to only run one test: '%s' ...\n", test_name);
        if (!found) {
            fprintf(stderr, "ERROR: No such test: '%s'\n", test_name);
            exit(1);
        }
	status = run_single_test(suite, test_name_as_symbol, reporter);
        free(test_name_as_symbol);
    } else {
        if (verbose) {
            if (number_of_matches != count(test_items))
                printf(" to run %d matching tests ...\n", number_of_matches);
            else
                printf(" to run all %d discovered tests ...\n", count(test_items));
        }
        status = run_test_suite(suite, reporter);
    }

    reflective_runner_cleanup(test_library_handle);

    for (int i = 0; test_items[i].symbol != NULL; ++i)
        free(test_items[i].symbol);
    return(status);
}


/*----------------------------------------------------------------------*/
static void register_test(TestItem *test_items, int maximum_number_of_tests, char *symbol) {
    int number_of_tests;

    for (number_of_tests = 0; test_items[number_of_tests].symbol != NULL; number_of_tests++);
    if (number_of_tests == maximum_number_of_tests) {
        fprintf(stderr, "\nERROR: Found too many tests (%d)! Giving up.\nConsider splitting tests between libraries on logical suite boundaries.\n", number_of_tests);
        exit(1);
    }

    test_items[number_of_tests].symbol = strdup(symbol);
    test_items[number_of_tests].context = suite_name_of_symbol(symbol);
    test_items[number_of_tests].test_name = test_name_of_symbol(symbol);
	test_items[number_of_tests+1].symbol = NULL;
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
static void discover_tests_in(const char* test_library, TestItem* test_items, const uint32_t maximum_number_of_test_items, bool verbose)
{
    char cmd[2048];
    strcpy(cmd, "/usr/bin/nm ");
    strcat(cmd, test_library);

    /* Open the command for reading. */
    FILE *nm_output_pipe = popen(cmd, "r");
    if (nm_output_pipe == NULL) {
        printf("\nERROR: Failed to run command ('/usr/bin/nm')\n" );
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line)-1, nm_output_pipe) != NULL) {
        if (is_cgreen_spec(line)) {
            char *symbol = strstr(line, NM_OUTPUT_COLUMN_SEPARATOR) + strlen(NM_OUTPUT_COLUMN_SEPARATOR);
            symbol[strlen(symbol) - 1] = 0; /* remove newline */
            if (verbose)
                printf("Discovered %s:%s\n", suite_name_of_symbol(symbol), test_name_of_symbol(symbol));
            register_test(test_items, maximum_number_of_test_items, symbol);
        }
    }

    pclose(nm_output_pipe);
}


/*======================================================================*/
int runner(TestReporter *reporter, const char *test_library, const char *test_name, bool verbose, bool no_run) {
    int status = 0;
    void *test_library_handle;
    const uint32_t MAXIMUM_NUMBER_OF_TESTS = 2048;
    TestItem discovered_tests[MAXIMUM_NUMBER_OF_TESTS];
    memset(discovered_tests, 0, sizeof(discovered_tests));

    discover_tests_in(test_library, discovered_tests, MAXIMUM_NUMBER_OF_TESTS, verbose);

    if (verbose)
        printf("Discovered %d tests\n", count(discovered_tests));

    if (!no_run) {
        if (verbose)
            printf("Opening [%s]", test_library);
        test_library_handle = dlopen (test_library, RTLD_NOW);
        if (test_library_handle == NULL) {
            fprintf (stderr, "\nERROR: dlopen failure (error: %s)\n", dlerror());
            exit(1);
        }
	status = run_tests(reporter, test_name, test_library_handle, discovered_tests, verbose);
    }
    return status;
}
