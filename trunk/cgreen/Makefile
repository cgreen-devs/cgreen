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

############# Internal

build:
	mkdir build
	cp Makefile.build build/Makefile


.SILENT:
