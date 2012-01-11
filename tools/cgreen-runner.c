
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

#include <cgreen/cgreen.h>
#include <cgreen/reporter.h>

#define MY_TEST_SUITE_NAME __func__

struct testlist_item {
    char name[128];
};

struct testlist_item testlist[1024];
int testlist_offset = 0;

void discover_tests();
int file_exists (const char *filename);

TestReporter* my_create_text_reporter(void *handle);
TestSuite *my_create_test_suite(void *handle);

int my_run_test_suite(void* handle, TestSuite *suite, TestReporter *reporter);
int my_run_single_test(void* handle, TestSuite *suite, const char* name, TestReporter *reporter);
void my_cleanup();

void add_discovered_tests_to_suite(void *handle, TestSuite* suite);

int main(int argc, char **argv) {
    int status;
    void *handle;

    if (argc < 2) {
        printf("Usage: cgreen-runner <test library filename>\n");
        exit(1);
    }

    const char* lib_filename = argv[1];

    if (!file_exists(lib_filename)) {
        printf("Couldn't find library: %s\n", lib_filename);
        exit(1);
    }

    discover_tests();

    printf("Discovered: %d tests\n", testlist_offset);

    handle = dlopen (lib_filename, RTLD_LAZY);
    if (!handle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }

    TestSuite *suite = my_create_test_suite(handle);
    TestReporter *reporter = my_create_text_reporter(handle);

    add_discovered_tests_to_suite(handle, suite);

    status = my_run_test_suite(handle, suite, reporter);

    my_cleanup(handle);

    return status;
}

int file_exists(const char *filename)
{
    return (access(filename, F_OK) == 0);
}
  
void my_cleanup(void *handle)
{
    dlclose(handle);
}

// XXX: hack to use nm command-line utility for now.  Use libelf later.
void discover_tests(const char* lib_filename)
{
    FILE *fp;
    char line[1024];
    char cmd[128];

    memset(cmd, 0, sizeof(cmd));
    strcat(cmd, "/usr/bin/nm ");
    strcat(cmd, lib_filename);

    /* Open the command for reading. */
    fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return;
    }

    memset(testlist, 0, sizeof(testlist));

    while (fgets(line, sizeof(line)-1, fp) != NULL) {
       char *match = strstr(line, "CgreenSpec");

       if (match != NULL) {
           size_t len = strlen(match);

           if (0 != strncmp(match, "CgreenSpec_default", strlen("CgreenSpec_default")))
           {
               strncpy(testlist[testlist_offset].name, match, len - 1);
               testlist_offset++;
           }
       }
    }

    pclose(fp);
}

void add_discovered_tests_to_suite(void *handle, TestSuite* suite)
{
    int i = 0;
    char *error;
    void (*cgreen_test)();
    void (*add_test_callback)(TestSuite *suite, const char *name, CgreenTest *test);

    add_test_callback = dlsym(handle, "add_test_");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }

    for (i = 0; i < testlist_offset; i++) {
        //printf("Discovered test %s ...\n", testlist[i].name);

        cgreen_test = dlsym(handle, testlist[i].name);

        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
 
        add_test_callback(suite, testlist[i].name, cgreen_test);
    }
}

TestSuite *my_create_test_suite(void *handle)
{
    TestSuite* (*callback)(const char *name);
    char *error;

    callback = dlsym(handle, "create_named_test_suite");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    return (*callback)(MY_TEST_SUITE_NAME);
}

TestReporter *my_create_text_reporter(void *handle)
{
    TestReporter* (*callback)();
    char *error;

    callback = dlsym(handle, "create_text_reporter");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    return (*callback)();
}

int my_run_test_suite(void* handle, TestSuite *suite, TestReporter *reporter)
{
    int (*callback)(TestSuite *suite, TestReporter *reporter);
    char *error;

    callback = dlsym(handle, "run_test_suite");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    return (*callback)(suite, reporter);
}

int my_run_single_test(void* handle, TestSuite *suite, const char* name, TestReporter *reporter)
{
    int (*callback)(TestSuite *suite, const char *name, TestReporter *reporter);
    char *error;

    callback = dlsym(handle, "run_single_test");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    return (*callback)(suite, name, reporter);
}

