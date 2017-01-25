#include <stdlib.h>
#include <string.h>
#include "memory.h"

#define MEMORY_INCREMENT 1024

struct MemoryPool_ {
    void** blocks;
    long size;
    long space;
};

static void enlarge(MemoryPool *pool);
static int move_to_front(MemoryPool *pool, void *pointer);
static void move_up_one(void **blocks, long amount);

MemoryPool *create_memory_pool() {
    MemoryPool *pool = (MemoryPool *)malloc(sizeof(MemoryPool));
    if (pool == NULL) {
        return NULL;
    }
    pool->blocks = (void **)malloc(MEMORY_INCREMENT * sizeof(void *));
    if (pool->blocks == NULL) {
        free(pool);
        return NULL;
    }
    pool->size = 0;
    pool->space = MEMORY_INCREMENT;
    return pool;
}

void free_memory_pool(MemoryPool *pool) {
    long i;
    for (i = 0; i < pool->size; i++) {
        free(pool->blocks[i]);
    }
    free(pool->blocks);
    free(pool);
}

void *memory_pool_allocate(MemoryPool *pool, size_t bytes) {
    enlarge(pool);
    return pool->blocks[pool->size++] = malloc(bytes);
}

void *memory_pool_reallocate(MemoryPool *pool, void *pointer, size_t bytes) {
    if (! move_to_front(pool, pointer)) {
        return NULL;
    }
    return pool->blocks[0] = realloc(pool->blocks[0], bytes);
}

static void enlarge(MemoryPool *pool) {
    if (pool->size == pool->space) {
        pool->blocks = (void**)realloc(pool->blocks, (pool->space + MEMORY_INCREMENT) * sizeof(void *));
        pool->space += MEMORY_INCREMENT;
    }
}

static int move_to_front(MemoryPool *pool, void *pointer) {
    long i;
    for (i = 1; i < pool->size; i++) {
        if (pool->blocks[i] == pointer) {
            move_up_one(pool->blocks, i);
            pool->blocks[0] = pointer;
            return 1;
        }
    }
    return 0;
}

/* static void move_up_one(void **blocks, long amount) { */
/*     if (amount == 0) { */
/*         return; */
/*     } */
/*     // FIXME: this doesn't compile and the semantics might be wrong */
/*     //memmove(blocks[0], blocks[1], (size_t)(blocks[amount] - blocks[1])); */
/* } */
