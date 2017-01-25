#ifndef PARAMETERS_HEADER
#define PARAMETERS_HEADER

#include <cgreen/vector.h>

/* Parameters are used from some user level tests so must be compilable in C++ */
#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

CgreenVector *create_vector_of_names(const char *parameters);
CgreenVector *create_vector_of_double_markers_for(const char *parameters);

#ifdef __cplusplus
    }
}
#endif

#endif
