CC=gcc
CFLAGS=-g

unit: unit.a text_reporter.a
	true

unit.a: unit.o reporter.o sequence.o
	ar -rs unit.a unit.o reporter.o sequence.o

text_reporter.a: text_reporter.o reporter.o
	ar -rs text_reporter.a text_reporter.o reporter.o

sample: unit sample.o unit.h text_reporter.h
	gcc sample.o unit.a text_reporter.a -o sample

test: sample
	./sample
