#include <cgreen/cgreen.h>
#include <cgreen/xml_reporter.h>
#include <cgreen/vector.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fnmatch.h>

#include "utils.h"
#include "runner.h"
#include "test_item.h"

#include "discoverer.h"

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
static char *context_name_of(const char* symbolic_name) {
    char *context_name;

    if (strchr(symbolic_name, ':')) {
        context_name = string_dup(symbolic_name);
        *strchr(context_name, ':') = '\0';
    } else {
        context_name = string_dup(CGREEN_DEFAULT_SUITE);
    }

    return context_name;
}


/*----------------------------------------------------------------------*/
static char *test_name_of(const char *symbolic_name) {
    const char *colon = strchr(symbolic_name, ':');
    if (colon) {
        return string_dup(colon+1);
    }

    return string_dup(symbolic_name);
}


/*----------------------------------------------------------------------*/
static bool test_matches_pattern(const char *symbolic_name_pattern,
                                 TestItem *test_item) {
    char* context_name = context_name_of(symbolic_name_pattern);
    int context_name_matches_test = fnmatch(context_name, test_item->context_name, 0) == 0;
    char* test_name = test_name_of(symbolic_name_pattern);
    int test_name_matches_test = fnmatch(test_name, test_item->test_name, 0) == 0;

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
static ContextSuite *add_new_context_suite(TestSuite *parent, const char* context_name,
                                           ContextSuite *next) {
    ContextSuite *new_context_suite = (ContextSuite *)calloc(1, sizeof(ContextSuite));
    new_context_suite->context_name = string_dup(context_name);
    new_context_suite->suite = create_named_test_suite(context_name);
    new_context_suite->next = next;
    add_suite_(parent, context_name, new_context_suite->suite);
    return new_context_suite;
}


/*----------------------------------------------------------------------*/
static void add_test_to_context(TestSuite *parent, ContextSuite **context_suites,
                                TestItem *test_item, CgreenTest *test) {
    TestSuite *suite_for_context = find_suite_for_context(*context_suites, test_item->context_name);

    if (suite_for_context == NULL) {
        *context_suites = add_new_context_suite(parent, test_item->context_name, *context_suites);
        suite_for_context = (*context_suites)->suite;
    }
    add_test_(suite_for_context, test_item->test_name, test);
}


/*----------------------------------------------------------------------*/
static TestItem *get_item_from(CgreenVector *tests, int i) {
    return (TestItem*)cgreen_vector_get(tests, i);
}


/*----------------------------------------------------------------------*/
static int add_matching_tests_to_suite(void *handle,
                                       const char *symbolic_name_pattern,
                                       CgreenVector *tests, TestSuite *suite,
                                       ContextSuite **context_suites)
{
    int count = 0;

    for (int i = 0; i<cgreen_vector_size(tests); i++) {
        if (symbolic_name_pattern == NULL || test_matches_pattern(symbolic_name_pattern,
                                                                  get_item_from(tests, i))) {
            char *error;
            const char *specification_name = get_item_from(tests, i)->specification_name;
            CgreenTest *test_function = (CgreenTest *)(dlsym(handle,
                                                             specification_name));
            if ((error = dlerror()) != NULL)  {
                PANIC("Could not find specification_name symbol '%s'",
                      specification_name);
                return -1;
            }

            add_test_to_context(suite, context_suites, get_item_from(tests, i), test_function);
            count++;
        }
    }

    return count;
}


/*----------------------------------------------------------------------*/
static bool matching_test_exists(const char *test_name, CgreenVector *tests) {
    for (int i = 0; i<cgreen_vector_size(tests); i++)
        if (test_matches_pattern(test_name, get_item_from(tests, i))) {
            return true;
        }
    return false;
}


/*----------------------------------------------------------------------*/
static int count(CgreenVector *vector) {
    return cgreen_vector_size(vector);
}

/*----------------------------------------------------------------------*/
static bool error_when_matching(int number_of_matches) {
    return number_of_matches < 0;
}


/*----------------------------------------------------------------------*/
static int run_tests(TestReporter *reporter,
                     const char *suite_name,
                     const char *symbolic_name,
                     void *test_library_handle,
                     CgreenVector *tests,
                     bool verbose) {
    int status;
    ContextSuite *context_suites = NULL;
    TestSuite *suite = create_named_test_suite(suite_name);

    const int number_of_matches = add_matching_tests_to_suite(test_library_handle,
                                                              symbolic_name,
                                                              tests,
                                                              suite,
                                                              &context_suites);
    if (error_when_matching(number_of_matches))
        return EXIT_FAILURE;

    if (symbolic_name != NULL && number_of_matches == 1) {
        bool found = matching_test_exists(symbolic_name, tests);
        if (verbose)
            printf(" to only run one test: '%s' ...\n", symbolic_name);
        if (!found) {
            fprintf(stderr, "ERROR: No such test: '%s' in '%s'\n", symbolic_name, suite_name);
            return EXIT_FAILURE;
        }
        char *test_name = test_name_of(symbolic_name);
        status = run_single_test(suite, test_name, reporter);
        free(test_name);
    } else {
        if (verbose) {
            if (number_of_matches != count(tests))
                printf(" to run %d matching tests ...\n", number_of_matches);
            else
                printf(" to run all %d discovered tests ...\n", count(tests));
        }

        if (number_of_matches > 0)
            status = run_test_suite(suite, reporter);
        else {
            fprintf(stderr, "ERROR: No such test: '%s' in '%s'\n", symbolic_name, suite_name);
            status = EXIT_FAILURE;
        }
    }

    destroy_test_suite(suite);
    destroy_context_suites(context_suites);
    return(status);
}


/*----------------------------------------------------------------------*/
const char *test_name_of_element(CgreenVector *tests, int index) {
    return get_item_from(tests, index)->test_name;
}

/*----------------------------------------------------------------------*/
static CgreenVector *sorted_test_items_from(CgreenVector *test_items) {
    CgreenVector *sorted = create_cgreen_vector((GenericDestructor)destroy_test_item);
    while (cgreen_vector_size(test_items) > 1) {
        int smallest = 0;
        const char *smallest_test_name = test_name_of_element(test_items, smallest);
        for (int i=1; i<cgreen_vector_size(test_items); i++) {
            const char *current_test_name = test_name_of_element(test_items, i);
            if (strcmp(smallest_test_name, current_test_name) > 0) {
                smallest = i;
                smallest_test_name = test_name_of_element(test_items, smallest);
            }
        }
        cgreen_vector_add(sorted, cgreen_vector_remove(test_items, smallest));
    }
    if (cgreen_vector_size(test_items) == 1) {
        cgreen_vector_add(sorted, cgreen_vector_remove(test_items, 0));
        destroy_cgreen_vector(test_items);
    }
    return sorted;
}

/*----------------------------------------------------------------------*/
static char *absolute(const char *file_path) {
    if (strchr("./", file_path[0]) != NULL)
        return strdup(file_path);
    else {
        char *tmp = malloc(strlen(file_path)+3);
        strcpy(tmp, "./");
        strcat(tmp, file_path);
        return tmp;
    }
}


/*======================================================================*/
int runner(TestReporter *reporter, const char *test_library_name,
           const char *suite_name, const char *test_name,
           bool verbose, bool dont_run) {
    int status = 0;
    void *test_library_handle = NULL;

    CgreenVector *tests = discover_tests_in(test_library_name, verbose);

    if (count(tests) == 0) {
        printf("No tests found in '%s'.\n", test_library_name);
        return 1;
    }

    if (verbose)
        printf("Discovered %d test(s)\n", count(tests));

    char *absolute_library_name = absolute(test_library_name);
    tests = sorted_test_items_from(tests);
    if (verbose)
        printf("Opening [%s]", test_library_name);
    test_library_handle = dlopen(absolute_library_name, RTLD_NOW);
    if (test_library_handle == NULL) {
        PANIC("dlopen failure when trying to run '%s' (error: %s)\n",
              absolute_library_name, dlerror());
        status = 2;
    } else {
        if (!dont_run) {
            status = run_tests(reporter, suite_name, test_name, test_library_handle,
                               tests, verbose);
        }
        dlclose(test_library_handle);
    }
    free(absolute_library_name);

    destroy_cgreen_vector(tests);

    return status;
}
