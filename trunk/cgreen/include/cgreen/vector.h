#ifndef _VECTOR_HEADER_
#define _VECTOR_HEADER_

typedef struct _Vector {
    int space;
    int size;
    void *items;
} Vector;

Vector *create_vector(void (*destructor)(void *));
void destroy_vector(Vector *vector);
void add_vector_item(Vector *vector, void *item);
void *remove_vector_item(Vector *vector, int position);
void get_from_vector(Vector *vector, int position);
int get_vector_size(Vector *vector);

#endif
