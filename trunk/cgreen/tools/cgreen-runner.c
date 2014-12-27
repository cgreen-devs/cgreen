#include <cgreen/cgreen.h>
#include "xml_reporter.h"
#include "utils.h"

#include <unistd.h>
#include <libgen.h>

#include "gopt.h"

#include "runner.h"


/*----------------------------------------------------------------------*/
static int file_exists(const char *filename)
{
    if (! filename) return 0;
    return (access(filename, F_OK) == 0);
}

/*----------------------------------------------------------------------*/
static void usage(const char **argv) {
    printf("Usage: %s [--xml <prefix>] [--suite <name>] [--verbose] [--no-run] [--help] (<library> [<test>])+\n\n", argv[0]);
    printf("Discover and run all or a single named cgreen test(s) from one or multiple\n");
    printf("dynamically loadable library.\n\n");
    printf("A single test can be run using the form [<context>:]<name> where <context> can\n");
    printf("be omitted if there is no context.\n\n");
    printf("--xml <prefix>\tInstead of messages on stdout, write results into one XML-file\n");
    printf("\t\tper suite, compatible with Hudson/Jenkins CI. The filename(s)\n");
    printf("\t\twill be '<prefix>-<suite>.xml'\n");
    printf("--suite <name>\tName the top level suite\n");
    printf("--no-run\tDon't run the tests\n");
    printf("--verbose\tShow progress information\n");
    printf("--colours/colors\tUse colours to emphasis result (requires ANSI-capable terminal)\n");
}


/*======================================================================*/

static void *options = NULL;
static TestReporter *reporter = NULL;
static TextReporterOptions reporter_options;

static void cleanup()
{
    if (reporter) destroy_reporter(reporter);
    if (options) free(options);
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


int main(int argc, const char **argv) {
    int status, i;

    bool verbose = false;
    bool no_run = false;

    const char *prefix_option;
    const char *suite_option = NULL;
    const char *tmp;

    atexit(cleanup);

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

    if (gopt_arg(options, 'x', &prefix_option))
        reporter = create_xml_reporter(prefix_option);
    else
        reporter = create_text_reporter();
    
    gopt_arg(options, 's', &suite_option);

    if (gopt_arg(options, 'v', &tmp))
        verbose = true;

    if (gopt_arg(options, 'n', &tmp))
        no_run = true;

    if (gopt_arg(options, 'c', &tmp))
        reporter_options.use_colours = true;
    else
        reporter_options.use_colours = false;

    if (gopt_arg(options, 'h', &tmp)) {
        usage(argv);
        return EXIT_SUCCESS;
    }

    if (argc < 2) {
        usage(argv);
        return EXIT_FAILURE;
    }


    set_reporter_options(reporter, &reporter_options);

    i = 1;
    while(i < argc) {
        const char *suite_name = suite_option;
        const char *test_name = NULL;
        const char *test_library = argv[i++];

        if (!file_exists(test_library)) {
            printf("Couldn't find library: %s\n", test_library);
            return EXIT_FAILURE;
        }

        suite_name = get_a_suite_name(suite_option, test_library);

        /* Check if the next argument is not a filename, thus a test name */
        if (!file_exists(argv[i])) {
            test_name = argv[i++];
        }

        status = runner(reporter, test_library, suite_name, test_name, verbose, no_run);
        if (status != 0) {
            printf("Library %s with test %s/%s resulted in error status: %i\n", test_library, suite_name, test_name, status);
            return status;
        }

        free((void*)suite_name);
    }
    
    return EXIT_SUCCESS;
}
