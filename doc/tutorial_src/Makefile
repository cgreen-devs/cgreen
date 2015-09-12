.IGNORE :
.SILENT :

CC = gcc -c -g -Wall
LINK = gcc -g
LIBDIRS = -L../../build/build-c/src
#LIBDIRS = -L/usr/local/lib

all: with_main with_runner

with_main: first0 first1 words0 words1 words2 words3 words4 words5 words6 words7 words8 words9 strlen1 strlen2 strlen3 strlen4 strlen5 strlen6 strlen7 schema1 schema2 crash1 crash2 crash3 suite1
	for f in $^ ; do echo $$f; ./$$f | sed 's/[0-9]*\.c/.c/g' > $$f.out; done

with_runner: runner1 runner2 stream0 stream1 stream2


clean:
	rm *.o *.out

first0: first_test0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

first1: first_test1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words0: all_tests.o words_test0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words1: all_tests.o words_test1.o words1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words2: all_tests.o words_test1.o words2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words3: all_tests.o words_test2.o words2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words4: all_tests.o words_test2.o words3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words5: all_tests.o words_test2.o words4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words6: all_tests.o words_test3.o words5.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words7: all_tests.o words_test3.o words6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words8: all_tests.o words_test4.o words6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words9: all_tests.o words_test4.o words7.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen1: strlen_test1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen2: strlen_test2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen3: strlen_test3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen4: strlen_test4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen5: strlen_test5.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen6: strlen_test6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen7: strlen_test7.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

schema1: schema_test1.o person.h
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen -lmysqlclient

schema2: schema_test2.o person.h
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen -lmysqlclient

crash1: crash_test1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

crash2: crash_test2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

crash3: crash_test3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

suite1: suite1.o suite_person_test.o suite_strlen_test.o 
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen -lmysqlclient

stream0: stream_test0.o stream1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

stream1: stream_test1.o stream1.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner stream_tests.so > $@.out

stream2: stream_test2.o stream1.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner stream_tests.so > $@.out

stream3: stream_test2.o stream2.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner stream_tests.so > $@.out

stream4: stream_test3.o stream2.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner stream_tests.so > $@.out

stream5: stream_test4.o stream2.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner stream_tests.so > $@.out

stream6: stream_test4.o stream3.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner stream_tests.so > $@.out

.PHONY: runner1
runner1: first_test2.o
	$(LINK) -shared -o first_test.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner first_test.so > $@.out

.PHONY: runner2
runner2: first_test2.o
	$(LINK) -shared -o first_test.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner first_test.so Cgreen:fails_this_test > $@.out

.PHONY: runner3
runner3: first_test2.o
	$(LINK) -shared -o first_test.so $^ $(LIBDIRS) -lcgreen
	cgreen-runner -v first_test.so Cgreen:fails_this_test > $@.out

