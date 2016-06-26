#include <cgreen/breadcrumb.h>
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

#include <cgreen/cdash_reporter.h>
#include "cdash_reporter_internal.h"


typedef time_t Timer(char *strtime);

typedef struct {
    CDashInfo *info;
    CDashPrinter *printer;
    FILE *stream;
    time_t begin;
    time_t startdatetime;
    time_t enddatetime;
    time_t teststarted;
    time_t testfinished;
} CDashMemo;

static void cdash_destroy_reporter(TestReporter *reporter);
static void cdash_reporter_start_suite(TestReporter *reporter, const char *name, const int number_of_tests);
static void cdash_reporter_start_test(TestReporter *reporter, const char *name);

static void cdash_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void cdash_show_pass(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);
static void show_incomplete(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments);

static void cdash_finish_test(TestReporter *reporter, const char *filename, int line,
                                             const char *message, uint32_t duration_in_milliseconds);
static void cdash_finish_suite(TestReporter *reporter, const char *filename, int line,
                                          uint32_t duration_in_milliseconds);

static time_t cdash_build_stamp(char *sbuildstamp, size_t sb);
static time_t cdash_current_time(char *strtime);
static double cdash_elapsed_time(time_t t1, time_t t2);

static CDashPrinter *cdash_print;

void set_cdash_reporter_printer(CDashPrinter *new_printer) {
    cdash_print = new_printer;
}


TestReporter *create_cdash_reporter(CDashInfo *info) {
    TestReporter *reporter;
    CDashMemo *memo;
    FILE *fd;
    char sbuildstamp[15];
    char strstart[30];
    char reporter_path[255];
    int rep_dir, strsize;


    if (!info)
        return NULL;

    reporter = create_reporter();
    if (!reporter)
        return NULL;

    memo = (CDashMemo *) calloc(1, sizeof(CDashMemo));
    if (!memo)
        return NULL;

    memo->info = info;
    cdash_print = fprintf;
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

    fprintf(fd, "%s\n%s\n", sbuildstamp, memo->info->type);

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
    memo->stream = fd;

    memo->startdatetime = cdash_current_time(strstart);

    cdash_print(memo->stream,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            " <Site BuildName=\"%s\" BuildStamp=\"%s-%s\" Name=\"%s\" Generator=\"%s\"\n"
            " OSName=\"%s\" Hostname=\"%s\" OSRelease=\"%s\"\n"
            " OSVersion=\"%s\" OSPlatform=\"%s\"\n"
            " Is64Bits=\"\" VendorString=\"\" VendorID=\"\"\n"
            " FamilyID=\"\" ModelID=\"\" ProcessorCacheSize=\"\" NumberOfLogicalCPU=\"\"\n"
            " NumberOfPhysicalCPU=\"\" TotalVirtualMemory=\"\" TotalPhysicalMemory=\"\"\n"
            " LogicalProcessorsPerPhysical=\"\" ProcessorClockFrequency=\"\" >\n"
            "  <Testing>\n"
            "   <StartDateTime>%s</StartDateTime>\n"
            "    <TestList>\n"
            "     <Test></Test>\n"
            "    </TestList>\n",
            memo->info->build, sbuildstamp, memo->info->type, memo->info->name, "Cgreen1.0.0",
            memo->info->os_name, memo->info->hostname, memo->info->os_release,
            memo->info->os_version, memo->info->os_platform, strstart);

    fflush(memo->stream);

    reporter->destroy = &cdash_destroy_reporter;
    reporter->start_suite = &cdash_reporter_start_suite;
    reporter->start_test = &cdash_reporter_start_test;
    reporter->show_fail = &cdash_show_fail;
    reporter->show_pass = &cdash_show_pass;
    reporter->show_incomplete = &show_incomplete;
    reporter->finish_test = &cdash_finish_test;
    reporter->finish_suite = &cdash_finish_suite;
    reporter->memo = memo;

    return reporter;
}

static void cdash_destroy_reporter(TestReporter *reporter) {
    char endtime[30];

    CDashMemo *memo = (CDashMemo *)reporter->memo;

    memo->enddatetime = cdash_current_time(endtime);

    cdash_print(memo->stream, "  <EndDateTime>%s</EndDateTime>\n"
            " <ElapsedMinutes>%.2f</ElapsedMinutes>\n"
            " </Testing>\n"
            "</Site>\n", endtime, cdash_elapsed_time(memo->startdatetime, memo->enddatetime));

    destroy_reporter(reporter);
}


static void cdash_reporter_start_suite(TestReporter *reporter, const char *name, const int number_of_tests) {
    (void)number_of_tests;

    reporter_start_test(reporter, name);
}

static void cdash_reporter_start_test(TestReporter *reporter, const char *name) {
    CDashMemo *memo = (CDashMemo *)reporter->memo;

    memo->teststarted = cdash_current_time(NULL);
    reporter_start_test(reporter, name);
}


static void print_test_header(CDashMemo *memo, const char* status, const char *name, const char* file, int line) {
     cdash_print(memo->stream,
                   "    <Test Status=\"%s\">\n"
                   "     <Name>%s</Name>\n"
                   "     <Path>%s</Path>\n"
                   "     <FullName>%s</FullName>\n"
                   "     <FullCommandLine>at [%s] line [%d]</FullCommandLine>\n",
                   status, name, file, file, file, line);
}

static void print_results_header(CDashMemo *memo, const char *name, float exectime) {
    cdash_print(memo->stream,
                  "     <Results>\n"
                  "      <NamedMeasurement type=\"numeric/double\" name=\"Execution Time\">\n"
                  "       <Value>%f</Value>\n"
                  "      </NamedMeasurement>\n"
                  "      <NamedMeasurement type=\"text/string\" name=\"Completion Status\">\n"
                  "       <Value>Completed</Value>\n"
                  "      </NamedMeasurement>\n"
                  "      <NamedMeasurement type=\"text/string\" name=\"Command Line\">\n"
                  "       <Value>%s</Value>\n"
                  "      </NamedMeasurement>\n",
                  exectime, name);
}

static void print_measurement(CDashMemo *memo, const char *value) {
    cdash_print(memo->stream,
                  "       <Measurement>\n"
                  "        <Value>%s</Value>\n"
                  "       </Measurement>\n",
                  value);
}

static void print_tail(CDashMemo *memo) {
    cdash_print(memo->stream,
                  "      </Results>\n"
                  "    </Test>\n");
}

static void cdash_show_fail(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    const char *name;
    char buffer[1000];
    float exectime;
    CDashMemo *memo;

    memo = (CDashMemo *)reporter->memo;

    memo->testfinished = cdash_current_time(NULL);

    exectime = (float)cdash_elapsed_time(memo->teststarted, memo->testfinished);

    name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);

    print_test_header(memo, "failed", name, file, line);
    print_results_header(memo, name, exectime);

    vsprintf(buffer, (message == NULL ? "Problem" : message), arguments);
    print_measurement(memo, buffer);

    print_tail(memo);
}

static void cdash_show_pass(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    double exectime;
    CDashMemo *memo;
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
    (void)message;
    (void)arguments;
    memo = (CDashMemo *)reporter->memo;

    memo->testfinished = cdash_current_time(NULL);
    exectime = cdash_elapsed_time(memo->teststarted, memo->testfinished);

    print_test_header(memo, "passed", name, file, line);
    print_results_header(memo, name, exectime);
    print_measurement(memo, "");
    print_tail(memo);
}



static void show_incomplete(TestReporter *reporter, const char *file, int line, const char *message, va_list arguments) {
    const char *name;
    char buffer[1000];
    float exectime;
    CDashMemo *memo;

    memo = (CDashMemo *)reporter->memo;

    memo->testfinished = cdash_current_time(NULL);

    exectime = (float)cdash_elapsed_time(memo->teststarted, memo->testfinished);

    name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);

    print_test_header(memo, "incomplete", name, file, line);
    print_results_header(memo, name, exectime);
   
    vsprintf(buffer, (message == NULL ? "Problem" : message), arguments);
    print_measurement(memo, buffer);

    print_tail(memo);
}


static void cdash_finish_test(TestReporter *reporter, const char *filename, int line,
                                             const char *message, uint32_t duration_in_milliseconds) {
    reporter_finish_test(reporter, filename, line, message, duration_in_milliseconds);
}


static void cdash_finish_suite(TestReporter *reporter, const char *filename, int line,
                                          uint32_t duration_in_milliseconds) {
    reporter_finish_test(reporter, filename, line, NULL, duration_in_milliseconds);
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

static double cdash_elapsed_time(time_t t1, time_t t2) {
    double diff;

    diff = difftime(t2, t1);
    return (diff == 0 ? 0 : (diff / 60));
}
