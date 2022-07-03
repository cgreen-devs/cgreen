# Makefile to run unit tests for cgreen-runner units
#
# We are using cgreen itself to test this but since the modules here
# might at some point get into the actual cgreen-runner there will be
# a clash when it loads the test library. So we will have to make the
# external symbols in the modules have other names when unittesting
# them using macro magic. The UNITTESTING preprocessor symbol will
# add the suffix '_unittesting' to externally, clashing symbols so
# that we can separate the ones in the cgreen-runner that is running
# the tests, and the ones in the units being tested.

UNAMEOEXISTS=$(shell uname -o 1>&2 2>/dev/null; echo $$?)
ifeq ($(UNAMEOEXISTS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif

CFLAGS = -Wall -g -I../include -MMD -DUNITTESTING -I.
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

# Use this if you want quick feed-back in this directory
tests: unit_tests acceptance_tests

#----------------------------------------------------------------------
unit unit_tests: discoverer_unit_tests runner_unit_tests
	LD_LIBRARY_PATH=. ./discoverer_unit_tests
	LD_LIBRARY_PATH=. ./runner_unit_tests

discoverer.o: CFLAGS += -DNM_EXECUTABLE='"nm"'

discoverer_unit_tests : discoverer_unit_tests.so
	$(CC) $(LDFLAGS) -o $@ $^

discoverer_unit_tests.so : discoverer_unit_test_runner.o discoverer.o test_item.o
	$(CC) $(LDFLAGS) -shared -o $@ $^ $(LIBRARIES)

# Auto-generate a runner for discoverer unittests
discoverer_unit_test_runner.c : discoverer_unit_tests.c
	echo "#include \"discoverer_unit_tests.c\"" > discoverer_unit_test_runner.c
	echo "TestSuite *discoverer_unit_tests() {" >> discoverer_unit_test_runner.c
	echo "    TestSuite *suite = create_test_suite();" >> discoverer_unit_test_runner.c
	grep Ensure discoverer_unit_tests.c | sed -e 's/Ensure(/    add_test_with_context(suite, /g' -e 's/) {/);/g' >> discoverer_unit_test_runner.c
	echo "    return suite;" >> discoverer_unit_test_runner.c
	echo "}" >> discoverer_unit_test_runner.c
	echo "int main(int argc, char **argv) {" >> discoverer_unit_test_runner.c
	echo "    return run_test_suite(discoverer_unit_tests(), create_text_reporter());" >> discoverer_unit_test_runner.c
	echo "}" >> discoverer_unit_test_runner.c

runner_unit_tests: runner_unit_tests.so
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

runner_unit_tests.so: runner_unit_test_runner.o test_item.o
	$(CC) $(LDFLAGS) -shared -o $@ $^ -ldl $(LIBRARIES)

# Auto-generate a runner for runner unittests
runner_unit_test_runner.c : runner_unit_tests.c
	echo "#include \"runner_unit_tests.c\"" > runner_unit_test_runner.c
	echo "TestSuite *runner_unit_tests() {" >> runner_unit_test_runner.c
	echo "    TestSuite *suite = create_test_suite();" >> runner_unit_test_runner.c
	grep Ensure runner_unit_tests.c | sed -e 's/Ensure(/    add_test_with_context(suite, /g' -e 's/) {/);/g' >> runner_unit_test_runner.c
	echo "    return suite;" >> runner_unit_test_runner.c
	echo "}" >> runner_unit_test_runner.c
	echo "int main(int argc, char **argv) {" >> runner_unit_test_runner.c
	echo "    return run_test_suite(runner_unit_tests(), create_text_reporter());" >> runner_unit_test_runner.c
	echo "}" >> runner_unit_test_runner.c

#----------------------------------------------------------------------
acceptance_tests: libdiscoverer_acceptance_tests.so
	$(CGREEN_RUNNER) $^

libdiscoverer_acceptance_tests.so: discoverer_acceptance_tests.o discoverer.o test_item.o io.o
	$(CC) -shared -o $@ $^ $(LIBRARIES)

#----------------------------------------------------------------------
clean:
	-rm *.o *.so *.dll

-include *.d
