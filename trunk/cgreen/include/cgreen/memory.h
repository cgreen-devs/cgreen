#ifndef MEMORY_HEADER
#define MEMORY_HEADER
#ifdef __cplusplus
    extern "C" {
#endif

typedef struct MemoryPool_ MemoryPool;

MemoryPool *create_memory_pool();
void free_memory_pool(MemoryPool *pool);
void *memory_pool_allocate(MemoryPool *pool, size_t bytes);
void *memory_pool_reallocate(MemoryPool *pool, void *pointer, size_t bytes);

#ifdef __cplusplus
    }
#endif
#endif
