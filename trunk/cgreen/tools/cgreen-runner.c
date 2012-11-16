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

    status = runner(reporter, test_library, test_name);

    return status;
}
