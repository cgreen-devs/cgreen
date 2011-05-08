#ifndef PARAMETERS_HEADER
#define PARAMETERS_HEADER

#include <cgreen/vector.h>

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

CgreenVector *create_vector_of_names(const char *parameters);

#ifdef __cplusplus
    }
}
#endif

#endif
