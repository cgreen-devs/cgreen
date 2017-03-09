#ifndef CIRCULAR_BUFFER_HEADER
#define CIRCULAR_BUFFER_HEADER

#define CB_TYPE int

struct CircularBuffer_ {
    CB_TYPE* buffer;
    CB_TYPE* buffer_end;
    CB_TYPE* head;
    CB_TYPE* tail;
};

typedef struct CircularBuffer_ CircularBuffer;

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

CircularBuffer* create_circular_buffer(int len);
void destroy_circular_buffer(CircularBuffer* cb);
int write_to_circular_buffer(CircularBuffer* cb, CB_TYPE data);

#ifdef __cplusplus
    }
}
#endif

#endif
