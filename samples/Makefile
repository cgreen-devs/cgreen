CC=gcc
CFLAGS=-g

sample: cgreen sample.o ../cgreen.h
	gcc sample.o ../cgreen.o -o sample

test: sample
	./sample

cgreen:
	cd ..; make

clean:
	rm -f *.o; true
	rm -f sample; true
	cd ..; make clean

