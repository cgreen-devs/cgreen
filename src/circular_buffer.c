#include <cgreen/internal/circular_buffer.h>
#include <stdlib.h>

CircularBuffer* create_circular_buffer(int len) {
    CircularBuffer* result;
    
    result = malloc(sizeof(CircularBuffer));
    if(!result)
        return NULL;
    result->buffer = malloc((len+1)*sizeof(CB_TYPE));
    if(!result) {
        free(result);
        return NULL;
    }
    result->buffer_end = result->buffer + (len+1);
    result->head = result->tail = result->buffer;
    return result;
}

void destroy_circular_buffer(CircularBuffer* cb) {
    free(cb->buffer);
    cb->buffer = NULL;
    free(cb);
}

int write_to_circular_buffer(CircularBuffer* cb, CB_TYPE data) {
    CB_TYPE* next = (cb->head+1 != cb->buffer_end) ? cb->head+1 : cb->buffer;
    if(next != cb->tail) {
        *(cb->head) = data;
        cb->head = next;
        return 1;
    } else
        return 0;
}

int read_from_circular_buffer(CircularBuffer* cb, CB_TYPE* pdata) {
    if(cb->tail == cb->head)
        return 0;
    *pdata = *(cb->tail++);
    if(cb->tail == cb-> buffer_end)
        cb->tail = cb-> buffer;
    return 1;
}

/* vim: set ts=4 sw=4 et cindent: */
