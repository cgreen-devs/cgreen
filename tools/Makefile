# Makefile to run unit tests for cgreen-runner units
#
# We are using cgreen itself to test this but since the modules here
# might at some point get into the actual cgreen-runner there will be
# an clash when it loads the test library. So we will have to make the
# external symbols in the modules have other names when unittesting
# them using some macro magic. But let's not worry about that for now,
# I'm confident that we can solve that...
#
# And I really want to use Cgreen!

COMMONFLAGS = -g
CFLAGS = $(COMMONFLAGS) -Wall -fPIC -I../include -MMD
LDFLAGS = $(COMMONFLAGS)

# NOTE: We should run these tests with a standard cgreen install, not
# with the cgreen-runner built in this tree.
LIBRARIES = -lcgreen

VPATH = ../src

# defer to CMake build from top level
all:
	make --no-print-directory -C ..

# Use this if you want quick feed back in this directory
tests: unit_tests acceptance_tests

#----------------------------------------------------------------------
main: main.o discoverer_acceptance_tests.o discoverer.o test_item.o io.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

#----------------------------------------------------------------------
unit_tests: libdiscoverer_unit_tests.so
	cgreen-runner $^

libdiscoverer_unit_tests.so: discoverer_unit_tests.o discoverer.o test_item.o
	$(CC) -shared -o $@ $^ $(LIBRARIES)

io.mocks : io.h
	../contrib/cgreen-mocker/cgreen-mocker.py -I../../pycparser-master/utils/fake_libc_include io.h > io.mocks

#----------------------------------------------------------------------
acceptance_tests: libdiscoverer_acceptance_tests.so
	cgreen-runner $^

libdiscoverer_acceptance_tests.so: discoverer_acceptance_tests.o discoverer.o test_item.o io.o
	$(CC) -shared -o $@ $^ $(LIBRARIES)

#----------------------------------------------------------------------
clean:
	-rm *.o *.so

-include *.d
