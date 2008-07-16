## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign

LIBS = -lm
INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/include

lib_LTLIBRARIES = libcgreen.la

libcgreen_la_SOURCES = unit.c messaging.c breadcrumb.c reporter.c assertions.c vector.c mocks.c constraint.c parameters.c text_reporter.c
libcgreen_la_LIBADD = $(LIBADD_DL)
libcgreen_la_LDFLAGS = -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@ -no-undefined

collector: collector.l vector.o slurp.o collector_test_list.o
	lex -B -t collector.l > collector.c
	$(CC) $(CFLAGS) collector.c vector.o slurp.o collector_test_list.o -o collector

#test: cgreen.a
#	cd tests; make all_tests
#	tests/all_tests

debug:
	$(MAKE) all CFLAGS="@DEBUG@"

profile:
	$(MAKE) all CFLAGS="@PROFILE@"
