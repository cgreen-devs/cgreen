#include <stdint.h>
#include "cgreen/internal/cgreen_time.h"

uint32_t cgreen_time_duration_in_milliseconds(uint32_t start_time_in_milliseconds,
                                              uint32_t end_time_in_milliseconds) {
    if (end_time_in_milliseconds < start_time_in_milliseconds) {
        return 0;
    }

    return end_time_in_milliseconds - start_time_in_milliseconds;
}
