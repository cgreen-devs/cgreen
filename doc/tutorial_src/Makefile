.IGNORE :
.SILENT :

CC = gcc -c -g -Wall
LINK = gcc -g
CGREEN_ROOT=../../build/build-c
LIBDIRS = -L$(CGREEN_ROOT)/src
RUNNER = PATH=$(CGREEN_ROOT)/src:$(PATH) $(CGREEN_ROOT)/tools/cgreen-runner
#LIBDIRS = -L/usr/local/lib

all: with_main with_runner

with_main: first0 first1 words0 words1 words2 words3 words4 words5 words6 words7 words8 words9 strlen1 strlen2 strlen3 strlen4 strlen5 strlen6 strlen7 schema1 schema2 crash1 crash2 crash3 suite1
	for f in $^ ; do echo $$f; ./$$f | sed 's/[0-9]*\.c/.c/g' > $$f.out; done

with_runner: runner1 runner2 \
			 stream0 stream1 stream2 stream3 stream4 stream5 stream6 \
			 multiple_streams1 multiple_streams2 \
			 formatter0 formatter1 formatter2 formatter3 formatter4 formatter5 formatter6 \
			 learning_mocks


clean:
	rm *.o *.out

first0: first_tests0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

first1: first_tests1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words0: all_tests.o words_tests0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words1: all_tests.o words_tests1.o words1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words2: all_tests.o words_tests1.o words2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words3: all_tests.o words_tests2.o words2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words4: all_tests.o words_tests2.o words3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words5: all_tests.o words_tests2.o words4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words6: all_tests.o words_tests3.o words5.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words7: all_tests.o words_tests3.o words6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words8: all_tests.o words_tests4.o words6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words9: all_tests.o words_tests4.o words7.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen1: strlen_tests1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen2: strlen_tests2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen3: strlen_tests3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen4: strlen_tests4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen5: strlen_tests5.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen6: strlen_tests6.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

strlen7: strlen_tests7.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

schema1: schema_tests1.o person.h
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen -lmysqlclient

schema2: schema_tests2.o person.h
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen -lmysqlclient

crash1: crash_tests1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

crash2: crash_tests2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

crash3: crash_tests3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

suite1: suite1.o suite_person_tests.o suite_strlen_tests.o 
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen -lmysqlclient

.PHONY: stream0
stream0: stream_tests0.o read_paragraph1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

# All these examples run using the runner which automatically outputs the file name
# Since that includes the example #, which we don't want in the tutorial, we use
# sed to remove that number from the file name in the output
.PHONY: stream1
stream1: stream_tests1.o read_paragraph1.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) stream_tests.so | sed 's/[0-9]*\.c/.c/g' > $@.out

.PHONY: stream2
stream2: stream_tests2.o read_paragraph1.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) stream_tests.so | sed 's/[0-9]*\.c/.c/g' > $@.out

.PHONY: stream3
stream3: stream_tests2.o read_paragraph2.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) stream_tests.so | sed 's/[0-9]*\.c/.c/g' > $@.out

.PHONY: stream4
stream4: stream_tests3.o read_paragraph2.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) stream_tests.so | sed 's/[0-9]*\.c/.c/g' > $@.out

.PHONY: stream5
# This test generates random characters read from memory so we need to tidy that up
stream5: stream_tests4.o read_paragraph2.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) stream_tests.so | sed 's/[0-9]*\.c/.c/g' | sed -e '/\"the$$/{ n; s/.*//; s//\"]/ }' > $@.out

.PHONY: stream6
stream6: stream_tests4.o read_paragraph3.o
	$(LINK) -shared -o stream_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) stream_tests.so | sed 's/[0-9]*\.c/.c/g' > $@.out

.PHONY: multiple_streams1
multiple_streams1: multiple_streams1.c
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: multiple_streams2
multiple_streams2: multiple_streams2.c
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: runner1
runner1: first_tests2.o
	$(LINK) -shared -o first_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) first_tests.so > $@.out

.PHONY: runner2
runner2: first_tests2.o
	$(LINK) -shared -o first_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) first_tests.so Cgreen:fails_this_test > $@.out

.PHONY: runner3
runner3: first_tests2.o
	$(LINK) -shared -o first_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) -v first_tests.so Cgreen:fails_this_test > $@.out

.PHONY: formatter0
formatter0: stream1.o formatter_tests0.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen

.PHONY: formatter1
formatter1: stream1.o formatter_tests1.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) formatter_tests.so > $@.out

formatter2: stream1.o formatter_tests2.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) formatter_tests.so > $@.out

formatter3: stream1.o formatter_tests3.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) formatter_tests.so > $@.out

formatter4: stream1.o formatter_tests4.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) formatter_tests.so > $@.out

formatter5: stream1.o formatter_tests5.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) formatter_tests.so > $@.out

formatter6: stream2.o formatter_tests5.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) formatter_tests.so > $@.out

learning_mocks: learning_mocks.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) $@.so
