#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#ifndef __cplusplus
void* cgreen_malloc(size_t size, const char * filename, int line);
#define malloc(size) cgreen_malloc(size, __FILE__, __LINE__)

void* cgreen_calloc(size_t num_members, size_t size, const char * filename, int line);
#define calloc(num_members, size) cgreen_calloc(num_members, size, __FILE__, __LINE__)

void* cgreen_realloc(void* pointer, size_t size, const char * filename, int line);
#define realloc(pointer, size) cgreen_realloc(pointer, size, __FILE__, __LINE__)

void cgreen_free(void* pointer, const char * filename, int line);
#define free(pointer) cgreen_free(pointer, __FILE__, __LINE__)

#else
#undef check_memory_leak
#define check_memory_leak(reporter)
#endif

#endif
