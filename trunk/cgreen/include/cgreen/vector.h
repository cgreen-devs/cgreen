#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct CgreenVector_ CgreenVector;

CgreenVector *create_cgreen_vector(void (*destructor)(void *));
void destroy_cgreen_vector(CgreenVector *vector);
void cgreen_vector_add(CgreenVector *vector, void *item);
void *cgreen_vector_remove(CgreenVector *vector, int position);
void *cgreen_vector_get(CgreenVector *vector, int position);
int cgreen_vector_size(CgreenVector *vector);

#ifdef __cplusplus
    }
#endif

#endif
