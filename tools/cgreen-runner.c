#include <cgreen/reporter.h>
#include <cgreen/runner.h>
#include <cgreen/suite.h>
#include <cgreen/text_reporter.h>
#include "xml_reporter.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "gopt.h"


typedef struct test_item {
    char *name;
} TestItem;

#define CGREEN_SPEC_PREFIX "CgreenSpec_"
#define CGREEN_SUITE_DEFAULT "default_"
#define CGREEN_NAME_SEPARATOR "_"

#if defined(__CYGWIN__) || defined(__APPLE__)
// Cygwin and MacOSX nm lists external names with a leading '_'
// which dlsym() doesn't want, so we'll include the '_' in the separator
#  define NM_OUTPUT_COLUMN_SEPARATOR " _"
#else
#  define NM_OUTPUT_COLUMN_SEPARATOR " "
#endif

static bool verbose = false;
static bool no_run = false;

/* Forward declarations: */
static uint32_t discover_tests_in(const char *, TestItem *, const uint32_t max);
static void add_discovered_tests_to_suite(void *handle, TestItem* tests, TestSuite* suite);
static int file_exists (const char *filename);
static void reflective_runner_cleanup(void*);
static char *mangle_test_name(const char *test_name);
static bool ensure_test_exists(TestItem discovered_tests[], char *test_name_with_prefix);


/*----------------------------------------------------------------------*/
static int run_tests(TestReporter *reporter, const char *test_name, void *test_library_handle, TestItem discovered_tests[], int number_of_tests) {
    int status;
    TestSuite *suite = create_named_test_suite("main");

    add_discovered_tests_to_suite(test_library_handle, discovered_tests, suite);

    if (test_name) {
        char *test_name_with_prefix = mangle_test_name(test_name);
        bool found = ensure_test_exists(discovered_tests, test_name_with_prefix);
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
        free(discovered_tests[i].name);
    return(status);
}



/*----------------------------------------------------------------------*/
static int cgreen(TestReporter *reporter, const char *test_library, const char *test_name) {
    int status = 0;
    void *test_library_handle;
    const uint32_t MAXIMUM_NUMBER_OF_TESTS = 2048;
    TestItem discovered_tests[MAXIMUM_NUMBER_OF_TESTS];
    memset(discovered_tests, 0, sizeof(discovered_tests));

    const uint32_t number_of_tests = discover_tests_in(test_library, discovered_tests, MAXIMUM_NUMBER_OF_TESTS);

    if (verbose)
        printf("Discovered: %d tests\n", number_of_tests);

    if (!no_run) {
        if (verbose)
            printf("Opening [%s]", test_library);
        test_library_handle = dlopen (test_library, RTLD_NOW);
        if (test_library_handle == NULL) {
            fprintf (stderr, "\nERROR: dlopen failure (error: %s)\n", dlerror());
            exit(1);
        }
	status = run_tests(reporter, test_name, test_library_handle, discovered_tests, number_of_tests);
    }
    return(status);
}


/*----------------------------------------------------------------------*/
static void usage(const char **argv) {
    printf("Usage: %s [--xml <prefix>] <library> [<name>]\n\n", argv[0]);
    printf("Discover and run all or a single named cgreen test(s) from a dynamically\n");
    printf("loadable library.\n\n");
    printf("--xml <prefix>\tInstead of messages on stdout, write results into one XML-file\n");
    printf("\t\tper suite, compatible with Hudson/Jenkins CI. The filename(s)\n");
    printf("\t\twill be '<prefix>-<suite>.xml'\n");
    printf("--no-run\tDon't run the tests\n");
    printf("--verbose\tShow progress information\n");
}


/*======================================================================*/
int main(int argc, const char **argv) {
    int status;

    /* TODO: in addition to running only a single named test, match tests with a substring and run only those */

    const char *test_library;
    const char *test_name = NULL;
    const char *prefix;
    const char *tmp;
    void *options = gopt_sort(&argc, argv, gopt_start(
                                                      gopt_option('x', 
                                                                  GOPT_ARG, 
                                                                  gopt_shorts('x'), 
                                                                  gopt_longs("xml")
                                                                  ),
                                                      gopt_option('v',
                                                                  GOPT_NOARG,
                                                                  gopt_shorts('v'),
                                                                  gopt_longs("verbose")
                                                                  ),
                                                      gopt_option('n',
                                                                  GOPT_NOARG,
                                                                  gopt_shorts('n'),
                                                                  gopt_longs("no-run")
                                                                  ),
                                                      gopt_option('h',
                                                                  GOPT_NOARG,
                                                                  gopt_shorts('h'),
                                                                  gopt_longs("help")
                                                                  )
                                                      )
                              );
    TestReporter *reporter;

    if (gopt_arg(options, 'x', &prefix))
        reporter = create_xml_reporter(prefix);
    else
        reporter = create_text_reporter();
    
    if (gopt_arg(options, 'v', &tmp))
        verbose = true;

    if (gopt_arg(options, 'n', &tmp))
        no_run = true;

    if (gopt_arg(options, 'h', &tmp)) {
        usage(argv);
        exit(0);
    }

    switch (argc) {
    case 3:
        test_name = argv[2];
    case 2:
        test_library = argv[1];
        break;
    default:
	usage(argv);
        return(0);
    }

    if (!file_exists(test_library)) {
        printf("Couldn't find library: %s\n", test_library);
        exit(1);
    }

    status = cgreen(reporter, test_library, test_name);

    return status;
}


/*----------------------------------------------------------------------*/
static char *mangle_test_name(const char *test_name) {
    char *test_name_with_prefixes = malloc(strlen(CGREEN_SPEC_PREFIX) +
                                           strlen(CGREEN_SUITE_DEFAULT) +
                                           strlen(test_name) + 1);
    strcpy(test_name_with_prefixes, CGREEN_SPEC_PREFIX);
    strcat(test_name_with_prefixes, CGREEN_SUITE_DEFAULT);
    strcat(test_name_with_prefixes, test_name);
    return test_name_with_prefixes;
}


/*----------------------------------------------------------------------*/
static bool ensure_test_exists(TestItem discovered_tests[], char *test_name_with_prefix) {
    for (int i = 0; discovered_tests[i].name != NULL; i++)
        if (strcmp(discovered_tests[i].name, test_name_with_prefix) == 0) {
            return true;
        }
    return false;
}

/*----------------------------------------------------------------------*/
static int file_exists(const char *filename)
{
    return (access(filename, F_OK) == 0);
}

/*----------------------------------------------------------------------*/
static void reflective_runner_cleanup(void *handle)
{
    dlclose(handle);
}

/*----------------------------------------------------------------------*/
static void register_test(TestItem *test_items, int maximum_number_of_tests, char *name) {
    int number_of_tests;

    for (number_of_tests = 0; test_items[number_of_tests].name != NULL; number_of_tests++);
    test_items[number_of_tests].name = strdup(name);

    if (number_of_tests == maximum_number_of_tests) {
        fprintf(stderr, "\nERROR: Found too many tests (%d)! Giving up.\nConsider splitting tests between libraries on logical suite boundaries.\n", number_of_tests);
        exit(1);
    }
}


/*----------------------------------------------------------------------*/
static const char *test_name(const char *name)
{
	return strstr(&name[strlen(CGREEN_SPEC_PREFIX)], CGREEN_NAME_SEPARATOR)+1;
}

/*----------------------------------------------------------------------*/
static const char *suite_name(const char *name)
{
	static char copy[1000];
	strcpy(copy, &name[strlen(CGREEN_SPEC_PREFIX)]);
	*strstr(copy, CGREEN_NAME_SEPARATOR) = '\0';
	return copy;
}

/*----------------------------------------------------------------------*/
// XXX: hack to use nm command-line utility for now.  Use libelf later.
// XXX: but nm is more portable across object formats...
static uint32_t discover_tests_in(const char* test_library, TestItem* test_items, const uint32_t maximum_number_of_test_items)
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
        char *match = strstr(line, NM_OUTPUT_COLUMN_SEPARATOR CGREEN_SPEC_PREFIX);
        if (match != NULL) {
            char *name = match + strlen(NM_OUTPUT_COLUMN_SEPARATOR);
            name[strlen(name) - 1] = 0; /* remove newline */
            if (verbose)
                printf("Discovered: %s %s\n", suite_name(name), test_name(name));
            register_test(test_items, maximum_number_of_test_items, name);
            number_of_tests++;
        }
    }

    pclose(nm_output_pipe);

    return number_of_tests;
}

/*----------------------------------------------------------------------*/
static void add_discovered_tests_to_suite(void *handle, TestItem *tests, TestSuite *suite)
{
    for (int i = 0; tests[i].name != NULL; i++) {
        char *error;
        CgreenTest *cgreen_test = (CgreenTest *)(dlsym(handle, tests[i].name));

        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }

        add_test_(suite, tests[i].name, cgreen_test);
    }
}
