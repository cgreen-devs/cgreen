#include <cgreen/cgreen.h>
#include "xml_reporter.h"

#include <unistd.h>

#include "gopt.h"

#include "runner.h"

static bool verbose = false;
static bool no_run = false;

/*----------------------------------------------------------------------*/
static int file_exists(const char *filename)
{
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
}


/*======================================================================*/

static void *options = NULL;
static TestReporter *reporter = NULL;

static void cleanup()
{
    if (reporter) destroy_reporter(reporter);
    if (options) free(options);
}

int main(int argc, const char **argv) {
    int status, i;

    const char *prefix;
    const char *suite_name = NULL;
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

    if (gopt_arg(options, 'x', &prefix))
        reporter = create_xml_reporter(prefix);
    else
        reporter = create_text_reporter();
    
    gopt_arg(options, 's', &suite_name);

    if (gopt_arg(options, 'v', &tmp))
        verbose = true;

    if (gopt_arg(options, 'n', &tmp))
        no_run = true;

    if (gopt_arg(options, 'h', &tmp)) {
        usage(argv);
	return EXIT_SUCCESS;
    }

    if (argc < 2) {
	usage(argv);
        return EXIT_FAILURE;
    }

    i = 1;
    while(i < argc) {
	const char *my_suite_name = suite_name;
	const char *test_name = NULL;
	const char *test_library = argv[i++];

	if (!file_exists(test_library)) {
	    printf("Couldn't find library: %s\n", test_library);
	    return EXIT_FAILURE;
	}

	if (my_suite_name == NULL)
	    my_suite_name = test_library;

	/* check if the next argument is not a filename, thus a test name */
	if (!file_exists(argv[i])) {
	    test_name = argv[i++];
	}

	status = runner(reporter, test_library, my_suite_name, test_name, verbose, no_run);
	if (status != 0) {
	    printf("library %s resulted in error status: %i\n", test_library, status);
	    return status;
	}
    }

    return EXIT_SUCCESS;
}
