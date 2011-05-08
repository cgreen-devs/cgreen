#include <cgreen/boxed_double.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
namespace cgreen {
#endif

/* NOTE: while returning BoxedDouble* here seems logical, it forces casts all over the place */
intptr_t box_double(double value) {
    BoxedDouble *box = (BoxedDouble *) malloc(sizeof(BoxedDouble));
    box->value = value;
    return (intptr_t)box;
}

double unbox_double(intptr_t box) {
    double value = as_double(box);
    free((BoxedDouble *)box);
    return value;
}

double as_double(intptr_t box) {
    return ((BoxedDouble *)box)->value;
}

#ifdef __cplusplus
} // namespace cgreen
#endif
