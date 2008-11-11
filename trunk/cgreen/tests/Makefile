CC=gcc
CFLAGS=-g -I../include/cgreen
LIBS=-lm
TEST_OBJECTS=all_tests.o breadcrumb_tests.o messaging_tests.o assertion_tests.o vector_tests.o constraint_tests.o parameters_test.o mocks_tests.o slurp_test.o

all_tests: ../src/cgreen.a $(TEST_OBJECTS) ../src/slurp.o
	gcc $(LIBS) $(TEST_OBJECTS) ../src/slurp.o ../src/cgreen.a -o all_tests

cgreen.a:
	cd ..; make cgreen.a

clean:
	rm -f *.o; true
	rm -f all_tests; true
