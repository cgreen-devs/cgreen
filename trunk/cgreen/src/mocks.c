#include "mocks.h"
#include "reporter.h"
#include <stdarg.h>

enum {playing = 1, recording};
static int record_state = playing;
static int always_state = 0;

void _compare_integer(const char *file, int line, const char *function, const char *parameter, int integer) {
}

void _compare_string(const char *file, int line, const char *function, const char *parameter, int string) {
}

void _compare_address(const char *file, int line, const char *function, const char *parameter, int address) {
}

void _result(const char *function) {
}

void _expect(const char *function, const char *file, int line) {
    record_state = recording;
    always_state = 0;
}

void _stub(const char *function, const char *file, int line) {
    record_state = recording;
    always_state = 0;
}

void _mock(const char *function, const char *file, int line) {
    record_state = recording;
    always_state = 0;
}

void _play() {
    record_state = playing;
    always_state = 0;
}

void _will_return(const char *function, ...) {
}

void _mask(const char *parameters) {
}

void _always() {
    always_state = 1;
}
