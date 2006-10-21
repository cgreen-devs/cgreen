#include "vector.h"
#include <stdlib.h>

static void increase_space(Vector *vector);

Vector *create_vector(void (*destructor)(void *)) {
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    vector->size = 0;
    vector->destructor = destructor;
    vector->space = 0;
    vector->items = NULL;
    return vector;
}

void destroy_vector(Vector *vector) {
    int i;
    if (vector->destructor != NULL) {
        for (i = 0; i < vector->size; i++) {
            (*vector->destructor)(vector->items[i]);
        }
    }
    free(vector->items);
    free(vector);
}

void vector_add(Vector *vector, void *item) {
    if (vector->size == vector->space) {
        increase_space(vector);
    }
    vector->items[vector->size] = item;
    vector->size++;
}

void *vector_remove(Vector *vector, int position) {
    void *item = vector->items[position];
    int i;
    for (i = position; i < vector->size; i++) {
        vector->items[i] = vector->items[i + 1];
    }
    vector->size--;
    return item;
}

void *vector_get(Vector *vector, int position) {
    return vector->items[position];
}

int vector_size(Vector *vector) {
    return vector->size;
}

static void increase_space(Vector *vector) {
    vector->space += 100;
    vector->items = realloc(vector->items, sizeof(void *) * vector->space);
}
