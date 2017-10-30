#ifndef UTILS_HEADER
#define UTILS_HEADER

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#define PANIC(...) panic(__FILE__, __LINE__, __VA_ARGS__)
        char *string_dup(const char *original);
        void panic_set_output_buffer(const char *buffer);
        void panic(const char *filename, int line, const char *fmt, ...);

#ifdef __cplusplus
    }
}
#endif

#endif
