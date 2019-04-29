#include "cgreen/internal/cgreen_time.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>

static LARGE_INTEGER qry_freq;
static bool qry_freq_initialized = false;

uint32_t cgreen_time_get_current_milliseconds() {
    if (!qry_freq_initialized) {
        QueryPerformanceFrequency(&qry_freq);
        qry_freq_initialized = true;
    }

    LARGE_INTEGER current_count;
    QueryPerformanceCounter(&current_count);

    current_count.QuadPart = current_count.QuadPart * 1000000 / qry_freq.QuadPart;

    return trunc(current_count.QuadPart / 1000);
}

/* vim: set ts=4 sw=4 et cindent: */
