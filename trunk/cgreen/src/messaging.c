#include <cgreen/messaging.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define message_content_size(Type) (sizeof(Type) - sizeof(long))

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
    CgreenMessageQueue *tmp;

    if (queue_count == 0) {
        atexit(&clean_up_messaging);
    }
    tmp = realloc(queues, sizeof(CgreenMessageQueue) * ++queue_count);
    if (tmp == NULL) {
      atexit(&clean_up_messaging);
      return -1;
    }
    queues = tmp;
    queues[queue_count - 1].queue = msgget((long)getpid(), 0666 | IPC_CREAT);
    queues[queue_count - 1].owner = getpid();
    queues[queue_count - 1].tag = tag;
    return queue_count - 1;
}

void send_cgreen_message(int messaging, int result) {
    CgreenMessage *message = malloc(sizeof(CgreenMessage));
    if (message == NULL) {
      return;
    }
    memset(message, 0, sizeof(*message));
    message->type = queues[messaging].tag;
    message->result = result;
    msgsnd(queues[messaging].queue, message, message_content_size(CgreenMessage), 0);
    free(message);
}

int receive_cgreen_message(int messaging) {
    CgreenMessage *message = malloc(sizeof(CgreenMessage));
    if (message == NULL) {
      return -1;
    }
    ssize_t received = msgrcv(queues[messaging].queue,
                              message,
                              message_content_size(CgreenMessage),
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
