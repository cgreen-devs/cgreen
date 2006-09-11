#include "messaging.h"
#include <sys/msg.h>
#include <stdlib.h>

static MessageQueue *ipc_list = NULL;
static int ipc_count = 0;

static void clean_up_ipc();

Message *create_message_buffer() {
	// There seems to be an "off by four" bug in the Linux implementation of messaging.
	// This can cause stack corruption, so we use the heap and add some padding.
    Message *message = (Message *)malloc(sizeof(Message) * 3);
    return message + 1;
}

void destroy_message_buffer(Message *message) {
    free(message - 1);
}

void *start_ipc() {
    if (ipc_count == 0) {
        atexit(&clean_up_ipc);
    }
    ipc_list = (MessageQueue *)realloc(ipc_list, sizeof(MessageQueue) * ++ipc_count);
    ipc_list[ipc_count - 1].queue = msgget((long)getpid(), 0666 | IPC_CREAT);
    ipc_list[ipc_count - 1].owner = getpid();
    return (void *)&(ipc_list[ipc_count - 1]);
}

void send_message(MessageQueue *messaging, int result) {
    Message *message = create_message_buffer();
    message->type = 1;
    message->result = result;
    msgsnd(messaging->queue, message, sizeof(Message), 0);
    destroy_message_buffer(message);
}

int receive_message(MessageQueue *messaging) {
    Message *message = create_message_buffer();
    ssize_t received = msgrcv(messaging->queue, message, sizeof(Message), 1, IPC_NOWAIT);
    int result = (received > 0 ? message->result : 0);
    destroy_message_buffer(message);
    return result;
}

static void clean_up_ipc() {
    int i;
    for (i = 0; i < ipc_count; i++) {
        if (ipc_list[i].owner == getpid()) {
            msgctl(ipc_list[i].queue, IPC_RMID, NULL);
        }
    }
    free(ipc_list);
    ipc_list = NULL;
    ipc_count = 0;
}
