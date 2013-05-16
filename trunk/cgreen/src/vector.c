#include <cgreen/vector.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
namespace cgreen {
#endif

struct CgreenVector_ {
    int size;
    void (*destructor)(void *);
    int space;
    void **items;
};

static void increase_space(CgreenVector *vector);

CgreenVector *create_cgreen_vector(void (*destructor)(void *)) {
    CgreenVector *vector = (CgreenVector *)malloc(sizeof(CgreenVector));
    vector->size = 0;
    vector->destructor = destructor;
    vector->space = 0;
    vector->items = NULL;
    return vector;
}

void destroy_cgreen_vector(CgreenVector *vector) {
    int i;
    if (vector->destructor != NULL) {
        for (i = 0; i < vector->size; i++) {
            (*vector->destructor)(vector->items[i]);
        }
    }

    free(vector->items);
    vector->items = NULL;
    vector->destructor = NULL;
    vector->size = 0;
    vector->space = 0;

    free(vector);
}

void cgreen_vector_add(CgreenVector *vector, void *item) {
    if (vector->size == vector->space) {
        increase_space(vector);
    }
    vector->items[vector->size] = item;
    vector->size++;
}

void *cgreen_vector_remove(CgreenVector *vector, int position) {
    void *item;
    int i;

    if (position < 0) {
        fprintf(stderr, "negative position disallowed in vector operation\n");
        return NULL;
    }

    item = vector->items[position];

    for (i = position; i < vector->size; i++) {
        vector->items[i] = vector->items[i + 1];
    }

    vector->items[vector->size] = NULL;

    vector->size--;
    return item;
}

void *cgreen_vector_get(const CgreenVector *vector, int position) {
    return vector->items[position];
}

int cgreen_vector_size(const CgreenVector *vector) {
    return (vector == NULL ? 0 : vector->size);
}

static void increase_space(CgreenVector *vector) {
    vector->space += 100;
    vector->items = (void**)realloc(vector->items, sizeof(void *) * vector->space);
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
