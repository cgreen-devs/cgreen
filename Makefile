CC=gcc
CFLAGS=-g -I./include 
LIBS=-lm
OBJECTS=src/unit.o src/messaging.o src/breadcrumb.o src/reporter.o \
        src/assertions.o src/vector.o src/mocks.o src/constraint.o \
        src/parameters.o src/text_reporter.o

all: clean libcgreen.a collector test

build: libcgreen.a collector all_tests

libcgreen.a: $(OBJECTS)  
	ar -rs src/libcgreen.a $(OBJECTS)
	cp src/libcgreen.a .

collector: src/collector

src/collector: src/collector.l src/vector.o src/slurp.o src/collector_test_list.o  
	lex -B -t src/collector.l > src/collector.c
	$(CC) $(CFLAGS) src/collector.c src/vector.o src/slurp.o src/collector_test_list.o -o src/collector

check: test

test: libcgreen.a all_tests
	cd tests; ./all_tests

all_tests:
	cd tests; make all_tests

clean:
	cd tests; make clean
	rm -f *.a; true
	rm -f src/*.o; true
	rm -f src/*.a; true
	rm -f src/collector.c; true
	rm -f src/collector; true

clean_test: clean test
