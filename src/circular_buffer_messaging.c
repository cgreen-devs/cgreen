#include <cgreen/messaging.h>
#include <cgreen/internal/circular_buffer.h>
#include <stdlib.h>

#define QUEUE_LENGTH 200
static int queue_count = 0;
static int* queue_tags = NULL;
static CircularBuffer** queues = NULL;

int start_cgreen_messaging(int tag) {
    const int queue = queue_count;
    queue_tags = realloc(queue_tags, (queue_count+1)*sizeof(int));
    queues = realloc(queues, (queue_count+1)*sizeof(CircularBuffer*));
#warning no alloc result checks
    queue_tags[queue] = tag;
    queues[queue] = create_circular_buffer(QUEUE_LENGTH);
#warning no alloc result checks

    queue_count++;
    return queue_count - 1;
}

void send_cgreen_message(int messaging, int result) {
    write_to_circular_buffer(queues[messaging], result);
#warning FIXME what if the circular buffer is full?
//    write_to_circular_buffer(queues[messaging], queue_tags[messaging]);
}

int receive_cgreen_message(int messaging) {
    int tag, result;
    if (!read_from_circular_buffer(queues[messaging], &result)) {
       return 0;
    }
//    read_from_circular_buffer(queues[messaging], &tag);
    return result;
}

/* vim: set ts=4 sw=4 et cindent: */
