CC=gcc
CFLAGS=-g

cgreen.a: unit.o messaging.o breadcrumb.o reporter.o sequence.o text_reporter.o
	ar -rs cgreen.a unit.o messaging.o breadcrumb.o reporter.o sequence.o text_reporter.o
