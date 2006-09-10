CC=gcc
CFLAGS=-g

cgreen.a: unit.o reporter.o sequence.o text_reporter.o
	ar -rs cgreen.a unit.o reporter.o sequence.o text_reporter.o

unit: unit.a text_reporter.a
	true

unit.a: unit.o reporter.o sequence.o
	ar -rs unit.a unit.o reporter.o sequence.o

text_reporter.a: text_reporter.o reporter.o
	ar -rs text_reporter.a text_reporter.o reporter.o
