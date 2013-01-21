# This Makefile ensures that the build is made out of source in a subdirectory called 'build'
# If it doesn't exist, it is created and a Makefile created (Makefile.build) to build for both C and C++
# This Makefile also contains delegation of the two most common make commands

all: build
	cd build; make

test: build
	cd build; make test

build:
	mkdir build
	cp Makefile.build build/Makefile

.SILENT:

