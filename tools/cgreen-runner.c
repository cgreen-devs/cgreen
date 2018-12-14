#include <cgreen/cgreen.h>
#include <cgreen/xml_reporter.h>

#include <cgreen/vector.h>

#include "utils.h"

#include <unistd.h>
#include <libgen.h>

#include "gopt.h"

#include "runner.h"
#include "discoverer.h"


/*----------------------------------------------------------------------*/
static int file_exists(const char *filename)
{
    if (! filename) return 0;
    return (access(filename, F_OK) == 0);
}


/*----------------------------------------------------------------------*/
static void version(void) {
    printf("cgreen-runner for Cgreen unittest and mocking framework v%s\n", VERSION);
    printf("Cgreen library version %s (%s) loaded\n", cgreen_library_version, cgreen_library_revision);
}


/*----------------------------------------------------------------------*/
static void usage(const char **argv) {
    printf("cgreen-runner for Cgreen unittest and mocking framework v%s\n\n", VERSION);
    printf("Usage:\n    %s [--xml <prefix>] [--suite <name>] [--verbose] [--quiet] [--no-run] [--help] (<library> [<test>])+\n\n", argv[0]);
    printf("Discover and run all or named cgreen test(s) from one or multiple\n");
    printf("dynamically loadable libraries.\n\n");
    printf("A single test can be run using the form [<context>:]<name> where <context> can\n");
    printf("be omitted if there is no context.\n\n");
    printf("  -c --colours/colors\t\tUse colours to emphasis result (requires ANSI-capable terminal)\n");
    printf("  -C --no-colours/no-colors\tDon't use colours\n");
    printf("  -x --xml <prefix>\t\tInstead of messages on stdout, write results into one XML-file\n");
    printf("\t\t\t\tper suite, compatible with Hudson/Jenkins CI. The filename(s)\n");
    printf("\t\t\t\twill be '<prefix>-<suite>.xml'\n");
    printf("  -s --suite <name>\t\tName the top level suite\n");
    printf("  -n --no-run\t\t\tDon't run the tests\n");
    printf("  -v --verbose\t\t\tShow progress information\n");
    printf("  -q --quiet\t\t\tJust output dots for each test\n");
    printf("     --version\t\t\tShow version information\n");
}


/*======================================================================*/

static void *options = NULL;
static TestReporter *reporter = NULL;
static TextReporterOptions reporter_options;

static void cleanup(void)
{
    if (reporter) reporter->destroy(reporter);
    if (options) gopt_free(options);
}

static char* get_a_suite_name(const char *suite_option, const char *test_library_name) {
    if (suite_option == NULL) {
        char *suite_name;
        char *s;
        /* basename can return the parameter or an internal static string. Work around this. */
        s = string_dup(test_library_name);
        suite_name = string_dup(basename(s));
        free(s);
        if ((s = strchr(suite_name, '.'))) {
            *s = '\0';
        }
        return suite_name;
    } else {
        return string_dup(suite_option);
    }
}


/*----------------------------------------------------------------------*/
static int initialize_option_handling(int argc, const char **argv) {
    options = gopt_sort(&argc, argv, gopt_start(
                                                gopt_option('x',
                                                            GOPT_ARG,
                                                            gopt_shorts('x'),
                                                            gopt_longs("xml")
                                                            ),
                                                gopt_option('s',
                                                            GOPT_ARG,
                                                            gopt_shorts('s'),
                                                            gopt_longs("suite")
                                                            ),
                                                gopt_option('v',
                                                            GOPT_NOARG,
                                                            gopt_shorts('v'),
                                                            gopt_longs("verbose")
                                                            ),
                                                gopt_option('V',
                                                            GOPT_NOARG,
                                                            gopt_shorts('V'),
                                                            gopt_longs("version")
                                                            ),
                                                gopt_option('c',
                                                            GOPT_NOARG,
                                                            gopt_shorts('c'),
                                                            gopt_longs("colours")
                                                            ),
                                                gopt_option('c',
                                                            GOPT_NOARG,
                                                            gopt_shorts('c'),
                                                            gopt_longs("colors")
                                                            ),
                                                gopt_option('C',
                                                            GOPT_NOARG,
                                                            gopt_shorts('C'),
                                                            gopt_longs("no-colors")
                                                            ),
                                                gopt_option('C',
                                                            GOPT_NOARG,
                                                            gopt_shorts('C'),
                                                            gopt_longs("no-colours")
                                                            ),
                                                gopt_option('q',
                                                            GOPT_NOARG,
                                                            gopt_shorts('q'),
                                                            gopt_longs("quiet")
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
    return(argc);
}


/*----------------------------------------------------------------------*/
static bool have_xml_option(void) {
    const char *prefix_option;

    return gopt_arg(options, 'x', &prefix_option);
}

/*----------------------------------------------------------------------*/
static bool run_tests_in_library(const char *suite_name_option, const char *test_name,
                                 const char *test_library, bool verbose, bool no_run) {
    int status;
    char *suite_name;

    suite_name = get_a_suite_name(suite_name_option, test_library);

    status = runner(reporter, test_library, suite_name, test_name, verbose, no_run);
    free((void*)suite_name);

    return status != 0;
}


/*----------------------------------------------------------------------*/
static void print_common_header(const char *suite_name_option, int library_count, int test_count) {
    char in_libraries_text[100] = "";
    sprintf(in_libraries_text, " in %d libraries", library_count);

    printf("Running \"%s\" (%d test%s%s)...\n", suite_name_option,
           test_count, test_count>1?"s":"",
           in_libraries_text);
}

/*----------------------------------------------------------------------*/
static void inhibit_appropriate_suite_message(int i, int library_count) {
    /* If we are using the text_reporter && a common suite name
       then inhibit the start suite message for all libraries and
       for all except the last library inhibit the finish suite
       message.  NOTE that the last library might be the second to
       last arg since there might be a testname pattern after it.
    */
    reporter_options.inhibit_start_suite_message = true;

    if (i < library_count-1 && library_count > 1) {
        reporter_options.inhibit_finish_suite_message = true;
    } else
        reporter_options.inhibit_finish_suite_message = false;
}



/*======================================================================*/
int main(int argc, const char **argv) {
    int i;

    bool verbose = false;
    bool no_run = false;

    const char *prefix_option;
    const char *suite_name_option = NULL;
    const char *tmp;

    bool any_fail = false;

    atexit(cleanup);

    argc = initialize_option_handling(argc, argv);

    if (gopt_arg(options, 'x', &prefix_option))
        reporter = create_xml_reporter(prefix_option);
    else
        reporter = create_text_reporter();

    gopt_arg(options, 's', &suite_name_option);

    if (gopt_arg(options, 'v', &tmp))
        verbose = true;

    if (gopt_arg(options, 'n', &tmp))
        no_run = true;

    reporter_options.use_colours = true;
    if (isatty(fileno(stdout))) {
        if (gopt_arg(options, 'C', &tmp))
            reporter_options.use_colours = false;
    } else {
        if (!gopt_arg(options, 'c', &tmp))
            reporter_options.use_colours = false;
    }

    if (gopt_arg(options, 'q', &tmp))
        reporter_options.quiet_mode = true;
    else
        reporter_options.quiet_mode = false;

    if (gopt_arg(options, 'h', &tmp)) {
        usage(argv);
        return EXIT_SUCCESS;
    }

    if (gopt_arg(options, 'V', &tmp)) {
        version();
        return EXIT_SUCCESS;
    }

    if (argc < 2) {
        usage(argv);
        return EXIT_FAILURE;
    }

    set_reporter_options(reporter, &reporter_options);

    /* Walk through all arguments and set up list of libraries and testnames */
    const char **libraries;       /* Array of libraries to run tests in */
    libraries = malloc(sizeof(char *)*argc);

    const char **testname;      /* Array of test name patterns, maybe NULL */
    testname = malloc(sizeof(char *)*argc);

    int library_count = 0;

    /* Find libraries */
    i = 1;
    while(i < argc) {
        libraries[library_count++] = argv[i++];

        /* Check if the next argument is not a filename, thus a test name,
           remember and move past it */
        if (!file_exists(argv[i])) {
            testname[library_count-1] = argv[i++];
        } else
            testname[library_count-1] = NULL;
    }

    reporter_options.inhibit_start_suite_message = false;
    reporter_options.inhibit_finish_suite_message = false;

    int test_count = 0;
    if (!have_xml_option() && suite_name_option != NULL) {

        /* Count all tests */
        for (i = 0; i<library_count; i++) {
            if (!file_exists(libraries[i])) {
                printf("Couldn't find library: %s\n", libraries[i]);
                break;
            }

            CgreenVector *discovered_tests = discover_tests_in(libraries[i], false);
            test_count += cgreen_vector_size(discovered_tests);
            destroy_cgreen_vector(discovered_tests);
        }
        print_common_header(suite_name_option, library_count, test_count);
    }


    for (i = 0; i<library_count; i++) {
        bool fail = false;

        if (!file_exists(libraries[i])) {
            printf("Couldn't find library: %s\n", libraries[i]);
            break;
        }

        if (!have_xml_option() && suite_name_option != NULL)
            inhibit_appropriate_suite_message(i, library_count);

        fail = run_tests_in_library(suite_name_option, testname[i], libraries[i],
                                    verbose, no_run);
        if (fail) any_fail = true;
    }

    if (!have_xml_option() && reporter_options.quiet_mode)
        printf("\n");

    free(libraries);
    free(testname);
    return any_fail?EXIT_FAILURE:EXIT_SUCCESS;
}
