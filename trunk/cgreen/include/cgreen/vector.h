#ifndef VECTOR_HEADER
#define VECTOR_HEADER

typedef struct Vector_ {
    int size;
    void (*destructor)(void *);
    int space;
    void **items;
} Vector;

Vector *create_vector(void (*destructor)(void *));
void destroy_vector(Vector *vector);
void vector_add(Vector *vector, void *item);
void *vector_remove(Vector *vector, int position);
void *vector_get(Vector *vector, int position);
int vector_size(Vector *vector);

#endif
