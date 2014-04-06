#include <cgreen/breadcrumb.h>
#include <cgreen/cdash_reporter.h>
#include <cgreen/reporter.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _MSC_VER
#include <wincompat.h>
#endif

#ifdef __cplusplus
namespace cgreen {
#endif

typedef int Printer(FILE *, const char *format, ...);
typedef time_t Timer(char *strtime);
typedef double DiffTimer(time_t t1, time_t t2);

typedef struct {
    CDashInfo *cdash;
    Printer *printer;
    Timer *timer;
    DiffTimer *difftimer;
    int  pipe_fd[2];
    time_t begin;
    time_t startdatetime;
    time_t enddatetime;
    time_t teststarted;
    time_t testfinished;
    FILE *f_reporter;
} CdashMemo;

static void cdash_destroy_reporter(TestReporter *reporter);
static void cdash_reporter_suite_started(TestReporter *reporter, const char *name, const int number_of_tests);
static void cdash_reporter_testcase_started(TestReporter *reporter, const char *name);

static void show_failed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_passed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);

static void cdash_reporter_testcase_finished(TestReporter *reporter, const char *filename, int line, const char *message);
static void cdash_reporter_suite_finished(TestReporter *reporter, const char *filename, int line);

static time_t cdash_build_stamp(char *sbuildstamp, size_t sb);
static time_t cdash_current_time(char *strtime);
static double cdash_enlapsed_time(time_t t1, time_t t2);

TestReporter *create_cdash_reporter(CDashInfo *cdash) {
    TestReporter *reporter;
    CdashMemo *memo;
    FILE *fd;
    char sbuildstamp[15];
    char strstart[30];
    char reporter_path[255];
    int rep_dir, strsize;

    if (!cdash)
        return NULL;

    reporter = create_reporter();
    if (!reporter)
        return NULL;

    memo = (CdashMemo *) malloc(sizeof(CdashMemo));
    if (!memo)
        return NULL;

    memo->cdash = (CDashInfo *) cdash;

    memo->printer = fprintf;
    memo->timer = cdash_current_time;
    memo->difftimer = cdash_enlapsed_time;
    memo->begin = cdash_build_stamp(sbuildstamp, 15);

    rep_dir = mkdir("./Testing", S_IXUSR|S_IRUSR|S_IWUSR|S_IXGRP|S_IRGRP|S_IXOTH);

    if (rep_dir) {
      if (errno != EEXIST) {
          free(memo);
          return NULL;
      }
    }

    fd = fopen("./Testing/TAG", "w+");
    if (fd == NULL) {
        free(memo);
        return NULL;
    }

    fprintf(fd,"%s\n%s\n", sbuildstamp, memo->cdash->type);

    fclose(fd);

    strsize = snprintf(reporter_path, sizeof(reporter_path) - 1, "./Testing/%s", sbuildstamp);

    rep_dir = mkdir(reporter_path, S_IXUSR|S_IRUSR|S_IWUSR|S_IXGRP|S_IRGRP|S_IXOTH);

    if (rep_dir) {
      if (errno != EEXIST) {
          free(memo);
          return NULL;
      }
    }

    snprintf( (char *) (reporter_path + strsize), (255 - strsize), "/Test.xml");

    fd = fopen(reporter_path, "w+");
    if (fd == NULL) {
      free(memo);
      return NULL;
    }

    /* now the Test.xml is in place */
    memo->f_reporter = fd;

    memo->startdatetime = cdash_current_time(strstart);

    memo->printer(memo->f_reporter,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            " <Site BuildName=\"%s\" BuildStamp=\"%s-%s\" Name=\"%s\" Generator=\"%s\""
            " OSName=\"%s\" Hostname=\"%s\" OSRelease=\"%s\""
            " OSVersion=\"%s\" OSPlatform=\"%s\""
            " Is64Bits=\"\" VendorString=\"\" VendorID=\"\""
            " FamilyID=\"\" ModelID=\"\" ProcessorCacheSize=\"\" NumberOfLogicalCPU=\"\""
            " NumberOfPhysicalCPU=\"\" TotalVirtualMemory=\"\" TotalPhysicalMemory=\"\""
            " LogicalProcessorsPerPhysical=\"\" ProcessorClockFrequency=\"\" >\n"
            "  <Testing>\n"
            "   <StartDateTime>%s</StartDateTime>\n"
            "    <TestList>\n"
            "     <Test></Test>\n"
            "    </TestList>\n",
            memo->cdash->build, sbuildstamp, memo->cdash->type, memo->cdash->name, "Cgreen1.0.0",
            memo->cdash->os_name, memo->cdash->hostname, memo->cdash->os_release,
            memo->cdash->os_version, memo->cdash->os_platform, strstart);

    fflush(memo->f_reporter);

    reporter->destroy = &cdash_destroy_reporter;
    reporter->start_suite = &cdash_reporter_suite_started;
    reporter->start_test = &cdash_reporter_testcase_started;
    reporter->show_fail = &show_failed;
    reporter->show_pass = &show_passed;
    reporter->show_incomplete = &show_incomplete;
    reporter->finish_test = &cdash_reporter_testcase_finished;
    reporter->finish_suite = &cdash_reporter_suite_finished;
    reporter->memo = memo;

    return reporter;
}

static void cdash_destroy_reporter(TestReporter *reporter) {
    char endtime[30];

    CdashMemo *memo = (CdashMemo *)reporter->memo;

    memo->enddatetime = cdash_current_time(endtime);

    memo->printer(memo->f_reporter, "  <EndDateTime>%s</EndDateTime>\n"
            " <ElapsedMinutes>%.2f</ElapsedMinutes>\n"
            " </Testing>\n"
            "</Site>\n", endtime, memo->difftimer(memo->startdatetime, memo->enddatetime));

    destroy_reporter(reporter);
}


static void cdash_reporter_suite_started(TestReporter *reporter, const char *name, const int number_of_tests) {
    (void)number_of_tests;

    reporter_start(reporter, name);
}

static void cdash_reporter_testcase_started(TestReporter *reporter, const char *name) {
    CdashMemo *memo = (CdashMemo *)reporter->memo;
    memo->teststarted = memo->timer(NULL);
    reporter_start(reporter, name);
}

static void show_failed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    const char *name;
    char buffer[1000];
    float exectime;
    CdashMemo *memo;

    memo = (CdashMemo *)reporter->memo;

    memo->testfinished = memo->timer(NULL);

    exectime = (float)memo->difftimer(memo->teststarted, memo->testfinished);

    name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);

    memo->printer(memo->f_reporter,
           "    <Test Status=\"failed\">\n");
    memo->printer(memo->f_reporter,
           "     <Name>%s</Name>\n"
           "      <Path>%s</Path>\n"
           "      <FullName>%s</FullName>\n"
           "      <FullCommandLine>at [%s] line [%d]</FullCommandLine>\n", name, file, file, file, line);
    memo->printer(memo->f_reporter,
           "      <Results>\n"
           "       <NamedMeasurement type=\"numeric/double\" name=\"Execution Time\"><Value>%f</Value></NamedMeasurement>\n"
           "       <NamedMeasurement type=\"text/string\" name=\"Completion Status\"><Value>Completed</Value></NamedMeasurement>\n"
           "       <NamedMeasurement type=\"text/string\" name=\"Command Line\"><Value>%s</Value></NamedMeasurement>\n"
           "       <Measurement>\n"
           "        <Value>", exectime, name);
    vsprintf(buffer, (message == NULL ? "Problem" : message), arguments);
    memo->printer(memo->f_reporter, "%s", buffer);
    memo->printer(memo->f_reporter, "</Value>\n"
           "       </Measurement>\n"
           "      </Results>\n"
           "    </Test>\n");
}

static void show_passed(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    double exectime;
    CdashMemo *memo;
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
    (void)message;
    (void)arguments;
    memo = (CdashMemo *)reporter->memo;

    memo->testfinished = memo->timer(NULL);
    exectime = memo->difftimer(memo->teststarted, memo->testfinished);

    memo->printer(memo->f_reporter,
           "    <Test Status=\"passed\">\n");
    memo->printer(memo->f_reporter, ""
           "     <Name>%s</Name>\n"
           "     <Path>%s</Path>\n"
           "     <FullName>%s</FullName>\n"
           "     <FullCommandLine>at [%s] line [%d]</FullCommandLine>\n", name, file, file, file, line);
    memo->printer(memo->f_reporter,
           "     <Results>\n"
           "      <NamedMeasurement type=\"numeric/double\" name=\"Execution Time\"><Value>%f</Value></NamedMeasurement>\n"
           "      <NamedMeasurement type=\"text/string\" name=\"Completion Status\"><Value>Completed</Value></NamedMeasurement>\n"
           "      <NamedMeasurement type=\"text/string\" name=\"Command Line\"><Value>%s</Value></NamedMeasurement>\n"
           "      <Measurement>\n"
           "       <Value></Value>\n"
           "      </Measurement>\n"
           "     </Results>\n"
           "    </Test>\n", exectime, name);
}

static void show_incomplete(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    (void)reporter;
    (void)file;
    (void)line;
    (void)message;
    (void)arguments;
}


static void cdash_reporter_testcase_finished(TestReporter *reporter, const char *filename, int line, const char *message) {
    reporter_finish(reporter, filename, line, message);
}


static void cdash_reporter_suite_finished(TestReporter *reporter, const char *filename, int line) {
    reporter_finish(reporter, filename, line, NULL);
}

static time_t cdash_build_stamp(char *sbuildstamp, size_t sb) {
    time_t t1;
    struct tm d1;
    char s[15];

    t1 = time(0);
    gmtime_r(&t1, &d1);

    strftime(s, sizeof(s), "%Y%m%d-%H%M", &d1);
    snprintf(sbuildstamp, sb, "%s", s);

    return t1;
}

static time_t cdash_current_time(char *strtime) {
    time_t t1;
    struct tm d1;
    char s[20];
    size_t i;

    t1 = time(0);
    gmtime_r(&t1, &d1);

    if(strtime == NULL)
        return t1;

    i = strftime(s, 20, "%b %d %H:%M EDT", &d1);

    strncpy(strtime, s, i+1);

    return t1;
}

static double cdash_enlapsed_time(time_t t1, time_t t2) {
    double diff;

    diff = difftime(t2, t1);
    return (diff == 0 ? 0 : (diff / 60));
}

#ifdef __cplusplus
} // namespace cgreen
#endif
