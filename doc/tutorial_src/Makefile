# This Makefile generates source and output that is part of Cgreen
# documentation. Files here are included in the asciidoc source so be
# careful with what you commit, it might thrash the documentation. In
# particular, be aware that non-UTF-8 characters in them (usually)
# makes generation of PDF impossible.  'stream2' and 'stream5' might
# generate random data as 'actual' values in the output file. That is
# the way it is supposed to be to work as documentation examples.
# Take care that data is printable but looks random.

CFLAGS = -Wall -g

.IGNORE :
.SILENT :

CGREEN_ROOT=../..
FPIC = -fPIC	# required on Linux, Darwin, ...

# Local tweaks, you can also override "MYSQLROOT={somedir}" on the command line
ifeq ($(shell uname),Darwin)
MYSQL_INCLUDE = /opt/local/include/mysql56/mysql
MYSQL_LIB = /opt/local/lib/mysql56/mysql
else
ifeq ($(shell uname -o),Cygwin)
FPIC=
MYSQL_INCLUDE = /usr/include/mysql
MYSQL_LIB = /usr/lib
else
MYSQLROOT = /usr
MYSQL_INCLUDE = $(MYSQLROOT)/include/mysql
MYSQL_LIB = $(MYSQLROOT)/lib
endif
endif

CGREEN_BIN=$(CGREEN_ROOT)/build
CC = gcc -c -g -Wall -I$(CGREEN_ROOT)/include -I$(CGREEN_ROOT)/src -I$(MYSQL_INCLUDE) $(FPIC)
LINK = gcc -g
LIBDIRS = -L$(CGREEN_BIN)/src -L$(MYSQL_LIB)

# You might need to fixup the various path issues to get dynamic loading to work.
# You could try the script ../setup.sh to ensure that the bins & libs in this
# development tree is used

# MacOSX:
LDPATH = DYLD_LIBRARY_PATH="$(CGREEN_BIN)/src:$(MYSQL_LIB)"
# Cygwin?
#LDPATH = PATH="$(CGREEN_BIN)/src:$(MYSQL_LIB)"

RUNNER = $(LDPATH) $(CGREEN_BIN)/tools/cgreen-runner

all: with_main with_runner special
	echo
	echo "NOTE: 'stream2.out' and 'stream5.out' should contain a simulation"
	echo "NOTE: of random memory as the actual value. Don't commit changes that do"
	echo "NOTE: not. But also note that PDF generation of the documentation"
	echo "NOTE: will fail if there are non-UTF-8 characters in them."

# Since we have multiple files for the same tutorial example, distinguished by a number,
# we need to remove that number from all output
REMOVE_NUMBERS=-e 's/\([a-z]\)[0-9]\+\"/\1\"/' -e 's/\([a-z]\)[0-9]\+\.c/\1\.c/'

# To minimize the diffs we also need to normalize execution times
NORMALIZE_DURATION=-e 's/\ in\ [0-9]\+ms/\ in\ 42ms/g'

# And batch that together in one symbol
SED=sed $(REMOVE_NUMBERS) $(NORMALIZE_DURATION)

with_main:  first0 first1 \
			words0 words1 words2 words3 words4 words5 words6 words7 words8 words9 \
			strlen1 strlen2 strlen3 strlen4 strlen5 strlen6 strlen7 \
			double1 \
			schema1 schema2 \
			crash1 crash2 crash3 \
			suite1 \
			test_as_xml0 \
			test_as_xml1 \
			test_as_xml2 \
			test_as_xml3 \
			test_as_xml4 \
			custom_constraint1 \
			custom_constraint2 \
			custom_constraint3
	for f in $^ ; do echo $$f; $(LDPATH) ./$$f | $(SED) > $$f.out; done

# Most tests built to run with the runner we can run here, but some need special care
# They have their output produced directly in their build rules below and are run using
# the 'special' rule
with_runner: side_effect stream0 stream1 stream2 stream3 stream4 stream6 \
			 multiple_streams1 multiple_streams2 \
			 formatter0 formatter1 formatter2 formatter3 formatter4 formatter5 formatter6
	for f in $^ ; do echo $$f; $(RUNNER) $$f.so | $(SED) > $$f.out; done

special: runner1 runner2 runner3 stream5 learning_mocks

clean:
	rm -f *.exe *.o *.out *.so *.stackdump *~

first_tests0.o: first_tests0.c
	$(CC) -o $@ -c $^ $(CFLAGS) -Wno-unused-variable

first0: first_tests0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

first1: first_tests1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

words_tests0.o: words_tests0.c
	$(CC) -o $@ -c $^ $(CFLAGS) -Wno-unused-variable

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

double1: double_tests1.o
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

test_as_xml0: test_as_xml0.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

test_as_xml1: test_as_xml1.o xml_reporter1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

test_as_xml2: test_as_xml1.o xml_reporter2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

test_as_xml3: test_as_xml1.o xml_reporter3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

test_as_xml4: test_as_xml1.o xml_reporter4.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

custom_constraint1: custom_constraint1.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

custom_constraint2: custom_constraint2.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen

custom_constraint3: custom_constraint3.o
	$(LINK) -o $@ $^ $(LIBDIRS) -lcgreen



###########################################################
# With runner
#
# All these examples run using the runner which automatically outputs the file name
# Since that includes the example #, which we don't want in the tutorial, we use
# sed to remove that number from the file name in the output
.PHONY: side_effect
side_effect: stream_tests0.o read_paragraph1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: stream0
stream0: stream_tests0.o read_paragraph1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: stream1
stream1: stream_tests1.o read_paragraph1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: stream2
stream2: stream_tests2.o read_paragraph1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: stream3
stream3: stream_tests2.o read_paragraph2.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: stream4
stream4: stream_tests3.o read_paragraph2.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: stream5
# This test generates random characters read from memory so we need to tidy that up
stream5: stream_tests4.o read_paragraph2.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) $@.so | $(SED) > $@.out
	echo "Trying to remove non-characters from stream5.out"
	echo "Check manually if the following sed fails:"
	cp $@.out $@.bak && LANG='' sed -e 's/^[^[[:space:]].*"]/"]/g' $@.bak > $@.bak2 && mv $@.bak2 $@.out
	rm $@.bak

.PHONY: stream6
stream6: stream_tests4.o read_paragraph3.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: multiple_streams1
multiple_streams1: multiple_streams1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: multiple_streams2
multiple_streams2: multiple_streams2.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

.PHONY: runner1
runner1: first_tests2.o
	$(LINK) -shared -o first_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) first_tests.so | $(SED) > $@.out

.PHONY: runner2
runner2: first_tests2.o
	$(LINK) -shared -o first_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) first_tests.so Cgreen:fails_this_test | $(SED) > $@.out

.PHONY: runner3
runner3: first_tests2.o
	$(LINK) -shared -o first_tests.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) -v first_tests.so Cgreen:fails_this_test | $(SED) > $@.out

.PHONY: formatter0
formatter0: stream1.o formatter_tests0.o
	$(LINK) -shared -o formatter_tests.so $^ $(LIBDIRS) -lcgreen

.PHONY: formatter1
formatter1: stream1.o formatter_tests1.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

formatter2: stream1.o formatter_tests2.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

formatter3: stream1.o formatter_tests3.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

formatter4: stream1.o formatter_tests4.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

formatter5: stream1.o formatter_tests5.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

formatter6: stream2.o formatter_tests5.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen

learning_mocks: learning_mocks.o
	$(LINK) -shared -o $@.so $^ $(LIBDIRS) -lcgreen
	$(RUNNER) $@.so
