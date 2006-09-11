#include "messaging.h"
#include <sys/msg.h>
#include <stdlib.h>

static MessageQueue *queues = NULL;
static int queue_count = 0;

static void clean_up_messaging();

Message *create_message_buffer() {
	// There seems to be an "off by four" bug in the Linux implementation of messaging.
	// This can cause stack corruption, so we use the heap and add some padding.
    Message *message = (Message *)malloc(sizeof(Message) * 3);
    return message + 1;
}

void destroy_message_buffer(Message *message) {
    free(message - 1);
}

void *start_messaging(int tag) {
    if (queue_count == 0) {
        atexit(&clean_up_messaging);
    }
    queues = (MessageQueue *)realloc(queues, sizeof(MessageQueue) * ++queue_count);
    queues[queue_count - 1].queue = msgget((long)getpid(), 0666 | IPC_CREAT);
    queues[queue_count - 1].owner = getpid();
    queues[queue_count - 1].tag = tag;
    return (void *)&(queues[queue_count - 1]);
}

void send_message(MessageQueue *messaging, int result) {
    Message *message = create_message_buffer();
    message->type = messaging->tag;
    message->result = result;
    msgsnd(messaging->queue, message, sizeof(Message), 0);
    destroy_message_buffer(message);
}

int receive_message(MessageQueue *messaging) {
    Message *message = create_message_buffer();
    ssize_t received = msgrcv(messaging->queue, message, sizeof(Message), messaging->tag, IPC_NOWAIT);
    int result = (received > 0 ? message->result : 0);
    destroy_message_buffer(message);
    return result;
}

static void clean_up_messaging() {
    int i;
    for (i = 0; i < queue_count; i++) {
        if (queues[i].owner == getpid()) {
            msgctl(queues[i].queue, IPC_RMID, NULL);
        }
    }
    free(queues);
    queues = NULL;
    queue_count = 0;
}
