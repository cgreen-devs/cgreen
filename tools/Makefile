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
# And I really want to use cgreen!

CFLAGS = -Wall -g -fPIC -I../include -MMD
VPATH = ../src

all: libdiscoverer_tests.so
	LD_LIBRARY_PATH=../build/src ../build/tools/cgreen-runner $^

libdiscoverer_tests.so: discoverer_tests.o discoverer.o test_item.o
	$(CC) -shared $(LDFLAGS) -o $@ $^

io.mocks : io.h
	../contrib/cgreen-mocker/cgreen-mocker.py -I../../pycparser-master/utils/fake_libc_include io.h > io.mocks

-include *.d
