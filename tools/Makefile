# Makefile to run unit tests for cgreen-runner units
#
# We are using cgreen itself to test this but since the modules here
# might at some point get into the actual cgreen-runner there will be
# a clash when it loads the test library. So we will have to make the
# external symbols in the modules have other names when unittesting
# them using macro magic. The UNITTESTING preprocessor symbol will
# add the suffix '_unittesting' to externally, clashing symbols so
# that we can separate the once in the cgreen-runner that is running
# the tests, and the ones in the units being tested.

UNAMEOEXISTS=$(shell uname -o 1>&2 2>/dev/null; echo $$?)
ifeq ($(UNAMEOEXISTS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif

COMMONFLAGS = -g
CFLAGS = $(COMMONFLAGS) -Wall -Wextra -I../include -MMD -DUNITTESTING
ifneq ($(OS),Cygwin)
  CFLAGS+=-fPIC
endif

LDFLAGS = $(COMMONFLAGS)

LIBRARIES = -L../build/src -lcgreen

VPATH = ../src

ifeq ($(OS),Darwin)
	LD_PATH_NAME=DYLD_LIBRARY_PATH
else
	LD_PATH_NAME=LD_LIBRARY_PATH
endif

CGREEN_RUNNER = $(LD_PATH_NAME)=../build/src cgreen-runner


# Redirect to CMake build from top level
all:
	make --no-print-directory -C ..

# Use this if you want quick feed back in this directory
tests: unit_tests acceptance_tests

#----------------------------------------------------------------------
main: main.o discoverer_acceptance_tests.o discoverer.o test_item.o io.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

utils.o: ../src/utils.c

#----------------------------------------------------------------------
unit_tests: libdiscoverer_unit_tests.so
	$(CGREEN_RUNNER) $^

libdiscoverer_unit_tests.so: discoverer_unit_tests.o discoverer.o test_item.o
	$(CC) -shared -o $@ $^ $(LIBRARIES)

io.mocks : io.h
	../contrib/cgreen-mocker/cgreen-mocker.py -I../../pycparser-master/utils/fake_libc_include io.h > io.mocks

#----------------------------------------------------------------------
acceptance_tests: libdiscoverer_acceptance_tests.so
	$(CGREEN_RUNNER) $^

libdiscoverer_acceptance_tests.so: discoverer_acceptance_tests.o discoverer.o test_item.o io.o
	$(CC) -shared -o $@ $^ $(LIBRARIES)

#----------------------------------------------------------------------
clean:
	-rm *.o *.so

-include *.d
