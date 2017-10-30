# This make file may serve as an example of how to easily build a lot
# of Cgreen test libraries and run them through the runner. By
# structuring it this way only affected libraries need to be rebuilt.
#
# Note that this is not official builds or tests for Cgreen. To do
# that you should go to root directory and do "make unit" or "make
# test"

CFLAGS = -Wall -fPIC -std=c99 -I../include -I.. -I../src

LIBRARY_PATH = ../build/src
ifneq ($(shell uname), Darwin)
LOAD_PATH = LD_LIBRARY_PATH=$(LIBRARY_PATH)
else
LOAD_PATH = DYLD_LIBRARY_PATH=$(LIBRARY_PATH)
endif

TEST_SOURCES = \
	assertion_tests.c \
	breadcrumb_tests.c \
	cdash_reporter_tests.c \
	cgreen_value_tests.c \
	constraint_tests.c \
	cute_reporter_tests.c \
	double_tests.c \
	environment_variables_tests.c \
	message_formatting_tests.c \
	messaging_tests.c \
	mocks_tests.c \
	parameters_tests.c \
	reflective_runner_no_teardown_tests.c \
	reflective_tests.c \
	text_reporter_tests.c \
	unit_tests.c \
	utils_tests.c \
	vector_tests.c \
	xml_reporter_tests.c \

TEST_LIBRARIES = $(patsubst %.c,lib%.so, $(TEST_SOURCES))

all: $(TEST_LIBRARIES)
	cd ..; make --no-print-directory
	$(LOAD_PATH) ../build/tools/cgreen-runner -s unittests $?

lib%_tests.so : %_tests.o ; $(CC) -shared -o $@ $^ -L$(LIBRARY_PATH) -lcgreen
