CC=gcc
CFLAGS=-g

all_tests: cgreen ../cgreen.h all_tests.o breadcrumb_tests.o messaging_tests.o assertion_tests.o vector_tests.o constraint_tests.o parameters_test.o mocks_tests.o
	gcc all_tests.o breadcrumb_tests.o messaging_tests.o assertion_tests.o vector_tests.o constraint_tests.o parameters_test.o mocks_tests.o ../cgreen.a -o all_tests

test: all_tests
	./all_tests

cgreen:
	cd ..; make
