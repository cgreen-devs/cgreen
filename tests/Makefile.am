## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign

TESTS = all_tests
check_PROGRAMS = all_tests
all_tests_SOURCES = all_tests.c breadcrumb_tests.c messaging_tests.c assertion_tests.c vector_tests.c constraint_tests.c parameters_test.c mocks_tests.c
# slurp_test.c
all_tests_LDFLAGS = 
all_tests_LDADD = -lm -lcgreen

INCLUDES = -I@includedir@
CFLAGS = -g 
AM_CPPFLAGS=

debug:
	$(MAKE) all CFLAGS="@DEBUG@"

profile:
	$(MAKE) all CFLAGS="@PROFILE@"

