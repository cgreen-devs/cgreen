CC=gcc
CFLAGS=-g

sample: cgreen sample.o ../cgreen.h
	gcc sample.o ../cgreen.a -o sample

test: sample
	./sample

cgreen:
	cd ..; make
