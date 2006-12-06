#include "parameters.h"
#include "vector.h"
#include <stdlib.h>

Vector *create_vector_of_names(const char *parameters) {
    return create_vector(&free);
}
