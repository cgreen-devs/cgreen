#include <cgreen/messaging.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

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

static void clean_up_messaging(void);

int start_cgreen_messaging(int tag) {
    CgreenMessageQueue *tmp;
    int pipes[2];

    if (queue_count == 0) {
        int atexit_result = atexit(&clean_up_messaging);

        if (atexit_result != 0) {
        	fprintf(stderr, "could not register clean up code\n");
        	return -1;
        }
    }

    tmp = (CgreenMessageQueue *) realloc(queues, sizeof(CgreenMessageQueue) * ++queue_count);
    if (tmp == NULL) {
    	/* ignoring return value here, as the world is ending anyways */
        (void)atexit(&clean_up_messaging);
        return -1;
    }
    queues = tmp;
    int pipe_result = pipe(pipes);
    if (pipe_result != 0) {
    	fprintf(stderr, "could not create pipes\n");
    	return -1;
    }

    queues[queue_count - 1].readpipe = pipes[0];
    queues[queue_count - 1].writepipe = pipes[1];
    queues[queue_count - 1].owner = getpid();
    queues[queue_count - 1].tag = tag;
    return queue_count - 1;
}

void send_cgreen_message(int messaging, int result) {
    CgreenMessage *message = (CgreenMessage *) malloc(sizeof(CgreenMessage));
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
    CgreenMessage *message = (CgreenMessage *) malloc(sizeof(CgreenMessage));
    if (message == NULL) {
      return -1;
    }

    if (0 != fcntl(queues[messaging].readpipe, F_SETFL, O_NONBLOCK)) {
    	fprintf(stderr, "could not set file status flag on read pipe\n");
    	free(message);
    	return -1;
    }

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

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
