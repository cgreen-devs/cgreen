CC=gcc
CFLAGS=-g -I../include
LIBS=-lm
TEST_OBJECTS=all_tests.o breadcrumb_tests.o messaging_tests.o assertion_tests.o vector_tests.o constraint_tests.o parameters_test.o mocks_tests.o slurp_test.o cute_reporter_tests.o collector_tests.o

all_tests: ../src/libcgreen.a $(TEST_OBJECTS) ../src/slurp.o
	gcc $(LIBS) $(TEST_OBJECTS) ../src/slurp.o ../src/libcgreen.a -o all_tests

cgreen.a:
	cd ..; make libcgreen.a

clean:
	rm -f *.o; true
	rm -f all_tests; true
