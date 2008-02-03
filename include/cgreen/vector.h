#ifndef VECTOR_HEADER
#define VECTOR_HEADER

typedef struct CgreenVector_ CgreenVector;

CgreenVector *create_vector(void (*destructor)(void *));
void destroy_vector(CgreenVector *vector);
void vector_add(CgreenVector *vector, void *item);
void *vector_remove(CgreenVector *vector, int position);
void *vector_get(CgreenVector *vector, int position);
int vector_size(CgreenVector *vector);

#endif
