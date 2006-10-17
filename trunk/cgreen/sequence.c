#include "sequence.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

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
	int length;
	union {
		int *integers;
		char **strings;
		void **addresses;
	} tape;
} Recording;

static Recording *all_recordings = NULL;
static int recording_count = 0;

static int get_sequence_iteration(const char *file, int line);
static void create_sequence(const char *file, int line);
static void create_counter(const char *file, int line, int expected);

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
	
}

void _replay_integer(const char *file, int line, int expectation) {
	
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
    free(all_counters);
    all_counters = NULL;
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
