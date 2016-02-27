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

unit:
	DIR=$$PWD/tests/ ; \
	for d in c c++ ; do \
	  cd build/build-$$d ; \
	  make ; \
	  tools/cgreen-runner -c `find tests -name *cgreen_tests* -type f -not -name *.a` ; \
	  tools/cgreen-runner -c `find tools/tests -name *cgreen_runner_tests.* -type f -not -name *.a` ; \
	  ../../tools/cgreen_runner_output_diff tools `find tests -name '*mock_messages*' -prune -type f -not -name '*.a' -not -name '*.error' -not -name '*.output'` mock_messages ../../tests/mock_messages.$$d.expected s%$$DIR%%g ; \
	  ../../tools/cgreen_runner_output_diff tools `find tests -name '*constraint_messages*' -prune -type f -not -name '*.a' -not -name '*.error' -not -name '*.output'` constraint_messages ../../tests/constraint_messages.$$d.expected s%$$DIR%%g ; \
	  CGREEN_PER_TEST_TIMEOUT=1 ../../tools/cgreen_runner_output_diff tools `find tests -name '*failure_messages*' -prune -type f -not -name '*.a' -not -name '*.error' -not -name '*.output'` failure_messages ../../tests/failure_messages.$$d.expected s%$${DIR}%%g ; \
	  cd ../.. ; \
	done

clean: build
	cd build; make clean

package: build
	cd build; make package

install:
	cd build; make install

############# Internal

build:
	mkdir build
	cp Makefile.build build/Makefile


.SILENT:
