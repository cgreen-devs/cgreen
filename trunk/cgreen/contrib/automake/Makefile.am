## Process this file with automake to produce Makefile.in

#libtool libraries (provides .la, .a and .so)
lib_LTLIBRARIES = libcgreen.la
libcgreen_la_LIBADD = -lm

#for a static lib comment _la_ stuff above and uncomment _a_ stuff below
#change in check target as well
#lib_LIBRARIES = libcgreen.a


#public headers installation in .../include/ i.e. cgreen.h into .../include/
include_HEADERS = include/cgreen/cgreen.h
#so that headers other than cgreen.h are installed under .../include/cgreen/
includesdir = $(includedir)/cgreen
includes_HEADERS = include/cgreen/unit.h include/cgreen/messaging.h include/cgreen/breadcrumb.h include/cgreen/reporter.h include/cgreen/assertions.h include/cgreen/vector.h include/cgreen/mocks.h include/cgreen/constraint.h include/cgreen/parameters.h include/cgreen/text_reporter.h include/cgreen/cute_reporter.h


#sources for libtool
libcgreen_la_SOURCES = src/assertions.c src/collector_test_list.c src/memory.c src/parameters.c src/text_reporter.c src/breadcrumb.c src/constraint.c src/messaging.c src/reporter.c src/unit.c src/cdash_reporter.c src/cute_reporter.c src/mocks.c src/slurp.c src/vector.c
libcgreen_la_CPPFLAGS = -I$(srcdir)/include
libcgreen_la_LDFLAGS = -version-info 1:0:0

#sources for static lib
#libcgreen_a_SOURCES = src/assertions.c src/collector_test_list.c src/memory.c src/parameters.c src/text_reporter.c src/breadcrumb.c src/constraint.c src/messaging.c src/reporter.c src/unit.c src/cdash_reporter.c src/cute_reporter.c src/mocks.c src/slurp.c src/vector.c
#libcgreen_a_CPPFLAGS = -I$(srcdir)/include

#check target
check_PROGRAMS = all_tests
all_tests_SOURCES = tests/all_tests.c tests/assertion_tests.c tests/breadcrumb_tests.c tests/collector_tests.c tests/constraint_tests.c tests/cute_reporter_tests.c tests/messaging_tests.c tests/mocks_tests.c tests/parameters_test.c tests/slurp_test.c tests/unit_tests.c tests/vector_tests.c
#when static lib change .la to .a
all_tests_LDADD = libcgreen.la -lm

#since the some_stuff file is not provided in svn repository
all_tests_CPPFLAGS = -I$(srcdir)/include -I$(srcdir) -DBINARYDIR="\"$(srcdir)\""
check_SCRIPTS = all_tests.sh

#run the tests when making check
TESTS = all_tests.sh

EXTRA_DIST = LICENSE README VERSION
