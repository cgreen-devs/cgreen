#include "sequence.h"
#include "reporter.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

enum {recording = 1, playing};

typedef struct _Sequence {
    int line;
    const char *file;
    int counter;
} Sequence;

static Sequence *all_sequences = NULL;
static int sequence_count = 0;

typedef struct _Counter {
    int line;
    const char *file;
    int expected;
    int actual;
} Counter;

static Counter *all_counters = NULL;
static int counter_count = 0;

typedef struct _Recording {
	int line;
	const char *file;
	union {
		int integer;
		char *string;
		void *address;
	} expected;
} Recording;

static Recording *all_recordings = NULL;
static int recording_count = 0;
static int recording_or_playing = recording;

static int get_sequence_iteration(const char *file, int line);
static void create_sequence(const char *file, int line);
static void create_counter(const char *file, int line, int expected);
static int create_recording(const char *file, int line);
static void destroy_recording(int position);
static int find_first_recording(const char *file, int line);

int _integer_sequence(const char *file, int line, ...) {
    int iteration = get_sequence_iteration(file, line);
    va_list arguments;
    va_start(arguments, line);
    while (iteration-- > 0) {
        va_arg(arguments, int);
    }
    int choice = va_arg(arguments, int);
    va_end(arguments);
    return choice;
}

char *_string_sequence(const char *file, int line, ...) {
    int iteration = get_sequence_iteration(file, line);
    va_list arguments;
    va_start(arguments, line);
    while (iteration-- > 0) {
        va_arg(arguments, char *);
    }
    char *choice = va_arg(arguments, char *);
    va_end(arguments);
    return choice;
}

void *_address_sequence(const char *file, int line, ...) {
    int iteration = get_sequence_iteration(file, line);
    va_list arguments;
    va_start(arguments, line);
    while (iteration-- > 0) {
        va_arg(arguments, void *);
    }
    void *choice = va_arg(arguments, void *);
    va_end(arguments);
    return choice;
}

void _count_calls(const char *file, int line, int expected) {
    int i;
    for (i = 0; i < counter_count; i++) {
        if (all_counters[i].line == line) {
            if (strcmp(all_counters[i].file, file) == 0) {
                all_counters[i].actual++;
                return;
            }
        }
    }
    create_counter(file, line, expected);
}

void replay() {
	recording_or_playing = playing;	
}

void _recorded_integer(const char *file, int line, int expected) {
	if (recording_or_playing == recording) {
	    int position = create_recording(file, line);
	    all_recordings[position].expected.integer = expected;
	} else {
	    int position = find_first_recording(file, line);
	    if (position < 0) {
	        (*get_test_reporter()->assert_true)(get_test_reporter(), file, line, 0, "Unexpected integer [%d]", expected);
	    } else {
	    	destroy_recording(position);
		}
	}
}

void walk_counters(void(*walker)(const char *, int, int, int, void *), void *memo) {
    int i;
    for (i = 0; i < counter_count; i++) {
        (*walker)(
                all_counters[i].file,
                all_counters[i].line,
                all_counters[i].expected,
                all_counters[i].actual,
                memo);
    }
}

void clear_sequences() {
    free(all_sequences);
    all_sequences = NULL;
    sequence_count = 0;
    free(all_counters);
    all_counters = NULL;
    counter_count = 0;
    free(all_recordings);
    all_recordings = NULL;
    recording_count = 0;
	recording_or_playing = recording;
}

static int get_sequence_iteration(const char *file, int line) {
    int i;
    for (i = 0; i < sequence_count; i++) {
        if (all_sequences[i].line == line) {
            if (strcmp(all_sequences[i].file, file) == 0) {
                return ++all_sequences[i].counter;
            }
        }
    }
    create_sequence(file, line);
    return 0;
}

static void create_sequence(const char *file, int line) {
    all_sequences = (Sequence *)realloc(all_sequences, sizeof(Sequence) * ++sequence_count);
    all_sequences[sequence_count - 1].line = line;
    all_sequences[sequence_count - 1].file = file;
    all_sequences[sequence_count - 1].counter = 0;
}

static void create_counter(const char *file, int line, int expected) {
    all_counters = (Counter *)realloc(all_counters, sizeof(Counter) * ++counter_count);
    all_counters[counter_count - 1].line = line;
    all_counters[counter_count - 1].file = file;
    all_counters[counter_count - 1].expected = expected;
    all_counters[counter_count - 1].actual = 1;
}

static int create_recording(const char *file, int line) {
    all_recordings = (Recording *)realloc(all_recordings, sizeof(Recording) * ++recording_count);
    all_recordings[recording_count - 1].line = line;
    all_recordings[recording_count - 1].file = file;
    return recording_count - 1;
}

static void destroy_recording(int position) {
	if (recording_count == 1) {
		free(all_recordings);
		all_recordings = NULL;
	} else {
		memmove(
				all_recordings + position + 1,
				all_recordings + position,
				(recording_count - position - 2) * sizeof(Recording));
	}
	recording_count--;
}

static int find_first_recording(const char *file, int line) {
    int i;
    for (i = 0; i < recording_count; i++) {
        if (all_recordings[i].line == line) {
            if (strcmp(all_recordings[i].file, file) == 0) {
                return i;
            }
        }
    }
    return -1;
}
