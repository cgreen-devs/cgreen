CC=gcc
CFLAGS=-g
OBJECTS=unit.o messaging.o breadcrumb.o reporter.o assertions.o vector.o mocks.o constraint.o parameters.o text_reporter.o

all: cgreen.o collector test

cgreen.o: cgreen.a
	ld -dn -s $(OBJECTS) -r -o cgreen.o

cgreen.a: $(OBJECTS)
	ar -rs cgreen.a $(OBJECTS)

collector: collector.l vector.o
	lex -B -t collector.l > collector.c
	$(CC) $(CFLAGS) collector.c vector.o -o collector

test: cgreen.o
	cd tests; make all_tests
	tests/all_tests

clean:
	cd tests; make clean
	rm -f *.o; true
	rm -f *.a; true

clean_test: clean test
