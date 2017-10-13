#ifndef UTILS_HEADER
#define UTILS_HEADER

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

        char *string_dup(const char *original);
        void panic_set_output_buffer(const char *buffer);
        void PANIC(const char *fmt, ...);

#ifdef __cplusplus
    }
}
#endif

#endif
