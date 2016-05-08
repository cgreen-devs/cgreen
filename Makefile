# This Makefile ensures that the build is made out of source in a
# subdirectory called 'build' If it doesn't exist, it is created.
#
# This Makefile also contains delegation of the most common make commands
#
# If you have cmake installed you should be able to do:
#
#	make
#	make test
#	make install
#	make package
#
# That should build Cgreen in the build directory, run some tests,
# install it locally and generate a distributable package.

all: build
	cd build; make

test: build
	cd build; make test

clean: build
	cd build; make clean

package: build
	cd build; make package

install:
	cd build; make install


# This is kind of a hack to get a quicker and clearer feedback when
# developing Cgreen Should be updated when new test libraries or
# output comparisons are added

# Find out if 'uname -o' works, if it does - use it, otherwise use 'uname -s'
UNAMEOEXISTS=$(shell uname -o 1>&2 2>/dev/null; echo $$?)
ifeq ($(UNAMEOEXISTS),0)
  OS=$(shell uname -o)
else
  OS=$(shell uname -s)
endif
ifeq ($(OS),Darwin)
	PREFIX=lib
	SUFFIX=.dylib
else ifeq ($(OS),Cygwin)
	PREFIX=cyg
	SUFFIX=.dll
else
	PREFIX=lib
	SUFFIX=.so
endif

OUTPUT_DIFF=../../tools/cgreen_runner_output_diff 
OUTPUT_DIFF_ARGUMENTS = $(1)_messages_tests \
	../../tests \
	$(1)_messages_tests.expected

unit: build
	SOURCEDIR=$$PWD/tests/ ; \
	cd build ; \
	make ; \
	export PATH=src:$$PATH ; \
	tools/cgreen-runner -c `find tests -name $(PREFIX)cgreen_c_tests$(SUFFIX)` ; \
	tools/cgreen-runner -c `find tests -name $(PREFIX)cgreen_cpp_tests$(SUFFIX)` ; \
	tools/cgreen-runner -c `find tools/tests -name $(PREFIX)cgreen_runner_tests$(SUFFIX)` ; \
	cd tests ; \
	$(OUTPUT_DIFF) $(call OUTPUT_DIFF_ARGUMENTS,mock) ; \
	$(OUTPUT_DIFF) $(call OUTPUT_DIFF_ARGUMENTS,constraint) ; \
	$(OUTPUT_DIFF) $(call OUTPUT_DIFF_ARGUMENTS,assertion) ; \
	CGREEN_PER_TEST_TIMEOUT=2 $(OUTPUT_DIFF) $(call OUTPUT_DIFF_ARGUMENTS,failure) ; \
	cd ../../..

############# Internal

build:
	mkdir build
	cd build; cmake ..

.SILENT:
