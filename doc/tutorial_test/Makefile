.IGNORE :
.SILENT :

CC = gcc -c -Wall
LINK = gcc
#LIBDIRS = -L../build/src
LIBDIRS = -L/usr/local/lib

all: run0 run1 run2 run3 run4 run5 run6 run7 run8 run9 run10 run11 run12 run13 run14 run15 run16 run17 run18 run19 run20 run21 run22 run23
	for f in $^ ; do echo "-----------------------------"; ./$$f ; done

clean:
	rm *.o

run0: first_test0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run1: first_test1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run2: all_tests.o words_test0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run3: all_tests.o words_test1.o words1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run4: all_tests.o words_test1.o words2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run5: all_tests.o words_test2.o words2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run6: all_tests.o words_test2.o words3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run7: all_tests.o words_test2.o words4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run8: all_tests.o words_test3.o words5.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run9: all_tests.o words_test3.o words6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run10: all_tests.o words_test4.o words6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run11: all_tests.o words_test4.o words7.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run12: strlen_test1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run13: strlen_test2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run14: strlen_test3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run15: strlen_test4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run16: crash_test1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run17: crash_test2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run18: stream_test1.o stream1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run19: stream_test2.o stream1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run20: stream_test2.o stream2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run21: stream_test3.o stream2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run22: stream_test4.o stream2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

run23: stream_test4.o stream3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

