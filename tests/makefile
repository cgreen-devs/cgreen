CC=gcc
CFLAGS=-g

all_tests: cgreen ../cgreen.h all_tests.o messaging_tests.o
	gcc all_tests.o messaging_tests.o ../cgreen.a -o all_tests

test: all_tests
	./all_tests

cgreen:
	cd ..; make
