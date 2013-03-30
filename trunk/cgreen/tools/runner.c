#include <cgreen/cgreen.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fnmatch.h>

#ifndef __cpluscplus
#include <stdbool.h>
#endif

#include "xml_reporter.h"
#include "runner.h"

/* The name of a test is either a named mangled from the name of the
   context, if any, and the actual test name.  Names (or patterns) on
   the command line is formatted as a symbolic name composed of the
   context name, a colon and the test name. The variable naming below
   is trying to be clear about which type of name it is. */

typedef struct test_item {
    char *specification_name;
    char *context_name;
    char *test_name;
} TestItem;


/* The ContextSuite is a datastructure created to partion the tests in
   suites according to the contexts, one suite per context. It
   consists of a list of nodes, each mapping a context name to a
   TestSuite. If there where a public way to navigate the structure of
   suites, we could name the TestSuites after the context and then
   find the appropriate suite by navigating in the TestSuite
   structure, but this seems impossible for now. */

typedef struct ContextSuite {
    char *context_name;
    TestSuite *suite;
    struct ContextSuite *next;
} ContextSuite;

static ContextSuite *context_suites = NULL;

/*----------------------------------------------------------------------*/
static void destroy_context_suites(ContextSuite *context_suite) {
    if (context_suite != NULL) {
        if (context_suite->next != NULL)
            destroy_context_suites(context_suite->next);
        free(context_suite->context_name);
        free(context_suite);
    }
}

#define CGREEN_DEFAULT_SUITE "default"


/*----------------------------------------------------------------------*/
static bool is_cgreen_spec(const char* line) {
    return strstr(line, CGREEN_SPEC_PREFIX) != NULL;
}


/*----------------------------------------------------------------------*/
static char *context_name_of(const char* symbolic_name) {
    char *context_name;

    if (strchr(symbolic_name, ':')) {
    	context_name = strdup(symbolic_name);
    	*strchr(context_name, ':') = '\0';
    } else {
        context_name = strdup(CGREEN_DEFAULT_SUITE);
    }

    return context_name;
}


/*----------------------------------------------------------------------*/
static char *test_name_of(const char *symbolic_name) {
    const char *colon = strchr(symbolic_name, ':');
    if (colon) {
        return strdup(colon+1);
    }

    return strdup(symbolic_name);
}


/*----------------------------------------------------------------------*/
static bool test_matches_pattern(const char *symbolic_name_pattern, TestItem test) {
    char* context_name = context_name_of(symbolic_name_pattern);
    int context_name_matches_test = fnmatch(context_name, test.context_name, 0) == 0;
    char* test_name = test_name_of(symbolic_name_pattern);
    int test_name_matches_test = fnmatch(test_name, test.test_name, 0) == 0;

    free(context_name);
    free(test_name);

    return context_name_matches_test && test_name_matches_test;
}


/*----------------------------------------------------------------------*/
static TestSuite *find_suite_for_context(ContextSuite *suites, const char *context_name) {
    ContextSuite *suite;

    for (suite = suites; suite != NULL; suite = suite->next) {
        if (strcmp(suite->context_name, context_name) == 0)
            break;
    }

    if (suite != NULL) {
        return suite->suite;
    }

    return NULL;
}


/*----------------------------------------------------------------------*/
static ContextSuite *add_new_context_suite(TestSuite *parent, const char* context_name, ContextSuite *next) {
    ContextSuite *new_context_suite = (ContextSuite *)calloc(1, sizeof(ContextSuite));
    new_context_suite->context_name = strdup(context_name);
    new_context_suite->suite = create_named_test_suite(context_name);
    new_context_suite->next = next;
    add_suite_(parent, context_name, new_context_suite->suite);
    return new_context_suite;
}


/*----------------------------------------------------------------------*/
static void add_test_to_context(TestSuite *parent, ContextSuite **context_suites, const char *context_name, const char *test_name, CgreenTest *test) {
    TestSuite *suite_for_context = find_suite_for_context(*context_suites, context_name);

    if (suite_for_context == NULL) {
	*context_suites = add_new_context_suite(parent, context_name, *context_suites);
	suite_for_context = (*context_suites)->suite;
    }
    add_test_(suite_for_context, test_name, test);
}


/*----------------------------------------------------------------------*/
static int add_matching_tests_to_suite(void *handle, const char *symbolic_name_pattern, TestItem *test_items, TestSuite *suite)
{
    int count = 0;

    for (int i = 0; test_items[i].specification_name != NULL; i++) {
        if (symbolic_name_pattern == NULL || test_matches_pattern(symbolic_name_pattern, test_items[i])) {
            char *error;
            CgreenTest *test_function = (CgreenTest *)(dlsym(handle, test_items[i].specification_name));

            if ((error = dlerror()) != NULL)  {
                fprintf (stderr, "%s\n", error);
                exit(1);
            }

            add_test_to_context(suite, &context_suites, test_items[i].context_name, test_items[i].test_name, test_function);
            count++;
        }
    }

    return count;
}


/*----------------------------------------------------------------------*/
static const char *position_of_context_name(const char *symbol) {
    const char *context_name = strstr(symbol, CGREEN_SPEC_PREFIX) +
        strlen(CGREEN_SPEC_PREFIX) +
        strlen(CGREEN_SEPARATOR);

    return context_name;
}


/*----------------------------------------------------------------------*/
static char *test_name_from_specname(const char *spec_name) {
    const char *context_name_position = position_of_context_name(spec_name);
    char *test_name_position = strdup(strstr(context_name_position, CGREEN_SEPARATOR) + strlen(CGREEN_SEPARATOR));

    *strstr(test_name_position, CGREEN_SEPARATOR) = '\0';
    return test_name_position;
}


/*----------------------------------------------------------------------*/
static char *context_name_from_specname(const char *symbol) {
    const char *context_name_position = position_of_context_name(symbol);
    char *context_name = strdup(context_name_position);
    *strstr(context_name, CGREEN_SEPARATOR) = '\0';

    return context_name;
}


/*----------------------------------------------------------------------*/
static bool matching_test_exists(const char *test_name, TestItem tests[]) {
    for (int i = 0; tests[i].specification_name != NULL; i++)
        if (test_matches_pattern(test_name, tests[i])) {
            return true;
        }
    return false;
}


/*----------------------------------------------------------------------*/
static void reflective_runner_cleanup(void *handle, TestItem test_items[]) {
    dlclose(handle);

    for (int i = 0; test_items[i].specification_name != NULL; ++i) {
        free(test_items[i].specification_name);
        free(test_items[i].context_name);
        free(test_items[i].test_name);
    }
}


/*----------------------------------------------------------------------*/
static int count(TestItem test_items[]) {
    int i;
    for (i = 0; test_items[i].specification_name != NULL; i++)
        ;
    return i;
}


/*----------------------------------------------------------------------*/
static int run_tests(TestReporter *reporter, const char *suite_name, const char *symbolic_name,
                     void *test_library_handle, TestItem test_items[], bool verbose) {
    int status;
    TestSuite *suite = create_named_test_suite(suite_name);

    int number_of_matches = add_matching_tests_to_suite(test_library_handle, symbolic_name, test_items, suite);

    if (symbolic_name != NULL && number_of_matches == 1) {
        bool found = matching_test_exists(symbolic_name, test_items);
        if (verbose)
            printf(" to only run one test: '%s' ...\n", symbolic_name);
        if (!found) {
            fprintf(stderr, "ERROR: No such test: '%s'\n", symbolic_name);
            return EXIT_FAILURE;
        }
        status = run_single_test(suite, test_name_of(symbolic_name), reporter);
    } else {
        if (verbose) {
            if (number_of_matches != count(test_items))
                printf(" to run %d matching tests ...\n", number_of_matches);
            else
                printf(" to run all %d discovered tests ...\n", count(test_items));
        }
        if (number_of_matches > 0)
            status = run_test_suite(suite, reporter);
        else {
            fprintf(stderr, "ERROR: No such test: '%s'\n", symbolic_name);
            status = EXIT_FAILURE;
        }
    }

    destroy_test_suite(suite);
    destroy_context_suites(context_suites);
    return(status);
}


/*----------------------------------------------------------------------*/
static void register_test(TestItem *test_items, int maximum_number_of_tests, char *specification_name) {
    int number_of_tests;

    for (number_of_tests = 0; test_items[number_of_tests].specification_name != NULL; number_of_tests++)
        ;
    if (number_of_tests == maximum_number_of_tests) {
        fprintf(stderr, "\nERROR: Found too many tests (%d)! Giving up.\nConsider splitting tests between libraries on logical suite boundaries.\n", number_of_tests);
        exit(1);
    }

    test_items[number_of_tests].specification_name = strdup(specification_name);
    test_items[number_of_tests].context_name = context_name_from_specname(specification_name);
    test_items[number_of_tests].test_name = test_name_from_specname(specification_name);
    test_items[number_of_tests+1].specification_name = NULL;
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
            char *specification_name = strstr(line, NM_OUTPUT_COLUMN_SEPARATOR) + strlen(NM_OUTPUT_COLUMN_SEPARATOR);
            specification_name[strlen(specification_name) - 1] = 0; /* remove newline */
            if (verbose) {
		char *suite_name = context_name_from_specname(specification_name);
                char *test_name = test_name_from_specname(specification_name);
                printf("Discovered %s:%s\n", suite_name, test_name);
                free(suite_name);
                free(test_name);
            }
            register_test(test_items, maximum_number_of_test_items, specification_name);
        }
    }

    pclose(nm_output_pipe);
}


/*======================================================================*/
int runner(TestReporter *reporter, const char *test_library_name,
           const char *suite_name, const char *test_name,
           bool verbose, bool dont_run) {
    int status = 0;
    void *test_library_handle;

    const uint32_t MAXIMUM_NUMBER_OF_TESTS = 2000; /* Some arbitrarily large number */
    TestItem discovered_tests[MAXIMUM_NUMBER_OF_TESTS];
    memset(discovered_tests, 0, sizeof(discovered_tests));

    discover_tests_in(test_library_name, discovered_tests, MAXIMUM_NUMBER_OF_TESTS, verbose);

    if (verbose)
        printf("Discovered %d test(s)\n", count(discovered_tests));

    if (!dont_run) {
        if (verbose)
            printf("Opening [%s]", test_library_name);
        test_library_handle = dlopen (test_library_name, RTLD_NOW);
        if (test_library_handle == NULL) {
            fprintf (stderr, "\nERROR: dlopen failure (error: %s)\n", dlerror());
            exit(1);
        }
        status = run_tests(reporter, suite_name, test_name, test_library_handle, discovered_tests, verbose);
    }

    reflective_runner_cleanup(test_library_handle, discovered_tests);

    return status;
}
