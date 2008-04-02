CC=gcc
CFLAGS=-g
OBJECTS=unit.o messaging.o breadcrumb.o reporter.o assertions.o vector.o mocks.o constraint.o parameters.o text_reporter.o

all: clean cgreen.a collector test

cgreen.a: $(OBJECTS)
	ar -rs cgreen.a $(OBJECTS)

collector: collector.l vector.o slurp.o collector_test_list.o
	lex -B -t collector.l > collector.c
	$(CC) $(CFLAGS) collector.c vector.o slurp.o collector_test_list.o -o collector

test: cgreen.a
	cd tests; make all_tests
	tests/all_tests

clean:
	cd tests; make clean
	rm -f *.o; true
	rm -f *.a; true

clean_test: clean test
