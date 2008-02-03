CC=gcc
CFLAGS=-g
OBJECTS=unit.o messaging.o breadcrumb.o reporter.o assertions.o vector.o mocks.o constraint.o parameters.o text_reporter.o

cgreen.o: cgreen.a
	ld -dn -s $(OBJECTS) -r -o cgreen.o

cgreen.a: $(OBJECTS)
	ar -rs cgreen.a $(OBJECTS)

test: clean cgreen.o
	cd tests; make all_tests
	tests/all_tests

clean:
	cd tests; make clean
	rm -f *.o; true
	rm -f *.a; true
