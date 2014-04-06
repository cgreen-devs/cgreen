#ifdef WIN32
#include "runner.h"
#include "cgreen/internal/runner_platform.h"
#include "cgreen/messaging.h"
#include "wincompat.h"

#ifdef __cplusplus
namespace cgreen {
#endif


#include "Strsafe.h"
#define SECOND  (1000)
static void run_named_test_child(TestSuite *suite, const char *name, TestReporter *reporter);
static void run_test_in_its_own_process_child(TestSuite *suite, CgreenTest *test, TestReporter *reporter);


static void CALLBACK stop(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    _exit(EXIT_SUCCESS);
}

void die_in(unsigned int seconds) {
    MMRESULT signal_result = timeSetEvent(seconds*SECOND,SECOND,&stop,0,TIME_ONESHOT);
    if (0 == signal_result){
        fprintf(stderr, "could not set alarm signal hander\n");
        return;
    }
}

static void run_test_in_the_current_process_child(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
    (*reporter->start_test)(reporter, test->name);
    run_the_test_code(suite, test, reporter);
    send_reporter_completion_notification(reporter);
    stop(0, 0, 0, 0, 0);
}

static void run_named_test_child(TestSuite *suite, const char *name, TestReporter *reporter) {
    int i;
    //(*reporter->start_suite)(reporter, suite->name, count_tests(suite));
    for (i = 0; i < suite->size; i++) {
        if (suite->tests[i].type == test_function) {
            if (strcmp(suite->tests[i].name, name) == 0) {
                run_test_in_the_current_process_child(suite, suite->tests[i].Runnable.test, reporter);
            }
        } else if (has_test(suite->tests[i].Runnable.suite, name)) {
            TestSuite* newSuite=suite->tests[i].Runnable.suite;
            (*suite->setup)();
            //moved recursive calls to start_suite and finish_suite to the caller, so I
            //can control the printf that occurs in the first one.
            //This may have undesireable side effects.  Not sure of the best solution
            (*reporter->start_suite)(reporter, newSuite->name, count_tests(newSuite));
            run_named_test_child(newSuite, name, reporter);
            (*reporter->finish_suite)(reporter, newSuite->filename, newSuite->line);
            (*suite->teardown)();
        }
    }
    send_reporter_completion_notification(reporter);
}

void run_specified_test_if_child(TestSuite *suite, TestReporter *reporter){
    //figure out if we are a child process, and if so, use the test name provided by our parent
    char testName[256];
    DWORD result = GetEnvironmentVariableA(CGREEN_TEST_TO_RUN,testName,sizeof(testName));

    if (result) //we are a child process, only run the specified test
    {
        //May have to deal with some issues here.  I don't call the function pointer for
        //reporter start and finish in order to avoid extraneous prints to the console,
        //but this may cause problems for other types of reporters.  Not sure the
        //best solution for this.
        reporter_start(reporter, testName);  //add breadcrumb without triggering output to console
        run_named_test_child(suite, testName, reporter);
        reporter_finish(reporter, suite->filename, suite->line, NULL);

        return; //never happens because we call stop inside run_named_test_child
    }

}

struct environment
{
    char env[MAX_PATH];
    char *p_head;
};

struct environment* create_environment()
{
    struct environment* p = (struct environment*)malloc(sizeof(struct environment));
    memset(p->env, 0, sizeof(p->env));
    p->p_head = p->env;
    return p;
}

static void AddEnvironmentVariable(struct environment* env,const char* varName, const char* valueString)
{
    size_t len;
    size_t envSize = MAX_PATH - (env->p_head-env->env);
    StringCbCopyA(env->p_head, envSize, varName);
    StringCbCatA(env->p_head, envSize, "=");
    StringCbCatA(env->p_head, envSize, valueString);
    StringCbCatA(env->p_head, envSize, "\0");
    len = strnlen_s(env->p_head, envSize);
    env->p_head += (len + 1);
}

static const char* get_environment_block(const struct environment* env)
{
    return env->env;
}

static void dispose_environment(struct environment* env)
{
    free(env);
}


void run_test_in_its_own_process(TestSuite *suite, CgreenTest *test, TestReporter *reporter) {
#define COMMAND_LINE_LENGTH (2*MAX_PATH)
    BOOL success;
    char fname[MAX_PATH+1]; //make enough room for full path  plus null terminator
    struct environment* p_environment = create_environment();
    char handleString[256];

    STARTUPINFOA siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;

    //get executable path
    GetModuleFileNameA(NULL, fname,MAX_PATH);

    //launch process
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);

    sprintf_s(handleString,sizeof(handleString), "%i", get_pipe_read_handle());
    AddEnvironmentVariable(p_environment, CGREEN_READ_HANDLE, handleString);

    sprintf_s(handleString,sizeof(handleString), "%i", get_pipe_write_handle());
    AddEnvironmentVariable(p_environment, CGREEN_WRITE_HANDLE, handleString);

    //put name of test to run in environment
    AddEnvironmentVariable(p_environment, CGREEN_TEST_TO_RUN, test->name);

    (*reporter->start_test)(reporter, test->name);
    //success = CreateProcessA(fname, NULL, NULL, NULL, true, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW , p_environment, NULL, &siStartupInfo, &piProcessInfo);
    success = CreateProcessA(fname, NULL, NULL, NULL, true, NORMAL_PRIORITY_CLASS , p_environment, NULL, &siStartupInfo, &piProcessInfo);
    dispose_environment(p_environment);
    WaitForSingleObject(piProcessInfo.hProcess,INFINITE);
    (*reporter->finish_test)(reporter, test->filename, test->line, NULL);

    return;
}


#ifdef __cplusplus
} // namespace cgreen
#endif

#endif
/* vim: set ts=4 sw=4 et cindent: */
