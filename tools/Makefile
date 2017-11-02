# Makefile to run unit tests for cgreen-runner units
# We are using cgreen itself to test this but
# since the modules here might at some point get into
# the actual cgreen-runner there will be an clash when
# it loads the test library. So we will have to make
# the external symbols in the modules have other names
# when unittesting them using some macro magic. But let's
# not worry about that for now, I'm confident that we
# can solve that...
#
# And I really want to use Cgreen!

COMMONFLAGS = -g
CFLAGS = $(COMMONFLAGS) -Wall -fPIC -I../include -MMD
LDFLAGS = $(COMMONFLAGS)

VPATH = ../src

all: unit_tests acceptance_tests main

#----------------------------------------------------------------------
main: main.o discoverer_acceptance_tests.o discoverer.o test_item.o io.o
	$(CC) $(LDFLAGS) -o $@ $^ -L../build/src -lcgreen

#----------------------------------------------------------------------
unit_tests: libdiscoverer_unit_tests.so
	LD_LIBRARY_PATH=../build/src ../build/tools/cgreen-runner $^

libdiscoverer_unit_tests.so: discoverer_unit_tests.o discoverer.o test_item.o
	$(CC) -shared -o $@ $^

io.mocks : io.h
	../contrib/cgreen-mocker/cgreen-mocker.py -I../../pycparser-master/utils/fake_libc_include io.h > io.mocks

#----------------------------------------------------------------------
acceptance_tests: libdiscoverer_acceptance_tests.so
	LD_LIBRARY_PATH=../build/src ../build/tools/cgreen-runner $^

libdiscoverer_acceptance_tests.so: discoverer_acceptance_tests.o discoverer.o test_item.o io.o
	$(CC) -shared -o $@ $^

#----------------------------------------------------------------------
clean:
	-rm *.o *.so

-include *.d
