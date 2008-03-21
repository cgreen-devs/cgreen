#include "messaging.h"
#include <sys/msg.h>
#include <stdlib.h>

typedef struct CgreenMessageQueue_ {
    int queue;
    pid_t owner;
    int tag;
} CgreenMessageQueue;

typedef struct CgreenMessage_ {
    long type;
    int result;
} CgreenMessage;

static CgreenMessageQueue *queues = NULL;
static int queue_count = 0;

static void clean_up_messaging();

int start_cgreen_messaging(int tag) {
    if (queue_count == 0) {
        atexit(&clean_up_messaging);
    }
    queues = (CgreenMessageQueue *)realloc(queues, sizeof(CgreenMessageQueue) * ++queue_count);
    queues[queue_count - 1].queue = msgget((long)getpid(), 0666 | IPC_CREAT);
    queues[queue_count - 1].owner = getpid();
    queues[queue_count - 1].tag = tag;
    return queue_count - 1;
}

void send_cgreen_message(int messaging, int result) {
    CgreenMessage *message = (CgreenMessage *)malloc(sizeof(CgreenMessage));
    message->type = queues[messaging].tag;
    message->result = result;
    msgsnd(queues[messaging].queue, message, sizeof(CgreenMessage) - sizeof(long), 0);
    free(message);
}

int receive_cgreen_message(int messaging) {
    CgreenMessage *message = (CgreenMessage *)malloc(sizeof(CgreenMessage));
    ssize_t received = msgrcv(queues[messaging].queue,
                              message,
                              sizeof(CgreenMessage) - sizeof(long),
                              queues[messaging].tag,
                              IPC_NOWAIT);
    int result = (received > 0 ? message->result : 0);
    free(message);
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
