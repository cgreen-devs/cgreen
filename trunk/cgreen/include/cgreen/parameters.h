#ifndef PARAMETERS_HEADER
#define PARAMETERS_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <cgreen/vector.h>

CgreenVector *create_vector_of_names(const char *parameters);

#ifdef __cplusplus
    }
#endif

#endif
