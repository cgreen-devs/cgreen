# This Makefile ensures that the build is made out of source in a subdirectory called 'build'
# If it doesn't exist, it is created and a Makefile created there (from Makefile.build)
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
# That should build cgreen for C and C++, run some tests, install it locally and
# generate two distributable packages.

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


# This is kind of a hack to get a quicker and clearer feedback when developing Cgreen
# Should be updated when new test libraries or output comparisons are added

# Find out if 'uname -o' works, if it does use it, otherwise use 'uname -s'
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

unit:
	DIR=$$PWD/tests/ ; \
	for d in c c++ ; do \
	  cd build/build-$$d ; \
	  make ; \
	  export PATH=src:$$PATH ; \
	  tools/cgreen-runner -c `find tests -name $(PREFIX)cgreen_tests$(SUFFIX)` ; \
	  tools/cgreen-runner -c `find tools/tests -name $(PREFIX)cgreen_runner_tests$(SUFFIX)` ; \
	  ../../tools/cgreen_runner_output_diff tools `find tests -name '$(PREFIX)mock_messages$(SUFFIX)'` \
		mock_messages ../../tests/mock_messages.$$d.expected s%$$DIR%%g ; \
	  ../../tools/cgreen_runner_output_diff tools `find tests -name '$(PREFIX)constraint_messages$(SUFFIX)'` \
		constraint_messages ../../tests/constraint_messages.$$d.expected s%$$DIR%%g ; \
	  CGREEN_PER_TEST_TIMEOUT=1 ../../tools/cgreen_runner_output_diff tools `find tests -name '$(PREFIX)failure_messages$(SUFFIX)'` \
		failure_messages ../../tests/failure_messages.$$d.expected s%$${DIR}%%g ; \
	  cd ../.. ; \
	done

############# Internal

build:
	mkdir build
	cp Makefile.build build/Makefile


.SILENT:
