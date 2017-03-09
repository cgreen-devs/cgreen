#include <stdlib.h>
#include <cgreen/internal/circular_buffer.h>

CircularBuffer* create_circular_buffer(int len) {
    CircularBuffer* result;
    
    result = malloc(sizeof(CircularBuffer));
    if(!result)
        return NULL;
    result->buffer = malloc(len*sizeof(CB_TYPE));
    if(!result) {
        free(result);
        return NULL;
    }
    result->buffer_end = result->buffer + len;
    result->head = result->tail = result->buffer;
    return result;
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

/* vim: set ts=4 sw=4 et cindent: */
