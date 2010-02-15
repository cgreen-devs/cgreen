#include <cgreen/messaging.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define message_content_size(Type) (sizeof(Type) - sizeof(long))

typedef struct CgreenMessageQueue_ {
    int readpipe;
    int writepipe;
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
    int pipes[2];

    if (queue_count == 0) {
        atexit(&clean_up_messaging);
    }
    tmp = realloc(queues, sizeof(CgreenMessageQueue) * ++queue_count);
    if (tmp == NULL) {
      atexit(&clean_up_messaging);
      return -1;
    }
    queues = tmp;
    pipe(pipes);
    queues[queue_count - 1].readpipe = pipes[0];
    queues[queue_count - 1].writepipe = pipes[1];
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
    write(queues[messaging].writepipe, message, sizeof(CgreenMessage));
    free(message);
}

int receive_cgreen_message(int messaging) {
    CgreenMessage *message = malloc(sizeof(CgreenMessage));
    if (message == NULL) {
      return -1;
    }
    fcntl(queues[messaging].readpipe, F_SETFL, O_NONBLOCK);
    ssize_t received = read(queues[messaging].readpipe, message, sizeof(CgreenMessage));
    int result = (received > 0 ? message->result : 0);
    free(message);
    return result;
}

static void clean_up_messaging() {
    int i;
    for (i = 0; i < queue_count; i++) {
        if (queues[i].owner == getpid()) {
	    close(queues[i].readpipe);
	    close(queues[i].writepipe);
        }
    }
    free(queues);
    queues = NULL;
    queue_count = 0;
}

/* vim: set ts=4 sw=4 et cindent: */
