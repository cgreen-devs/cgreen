#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <stdbool.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

        extern bool panic_use_colours;

#define PANIC(...) panic(__FILE__, __LINE__, __VA_ARGS__)
        extern char *string_dup(const char *original);
        extern void panic_set_output_buffer(const char *buffer);
        extern void panic(const char *filename, int line, const char *fmt, ...);

#ifdef __cplusplus
    }
}
#endif

#endif
