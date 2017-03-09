#ifndef CIRCULAR_BUFFER_HEADER
#define CIRCULAR_BUFFER_HEADER

#define CB_TYPE int

struct CircularBuffer_ {
    int length;
    CB_TYPE* buffer;
};

typedef struct CircularBuffer_ CircularBuffer;

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

CircularBuffer* create_circular_buffer(int len);

#ifdef __cplusplus
    }
}
#endif

#endif
