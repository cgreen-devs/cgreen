#include <cgreen/messaging.h>
#include <cgreen/internal/cgreen_pipe.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _MSC_VER
#include <wincompat.h>
//disable warning on windows
//'getpid','write': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _getpid, _write
#pragma warning(disable:4996)
#else
#include <sched.h>
#endif

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

int get_pipe_read_handle(void)
{
    if (queue_count)
    {
        return queues[queue_count - 1].readpipe;
    }

    return 0;
}

int get_pipe_write_handle(void)
{
    if (queue_count)
    {
        return queues[queue_count - 1].writepipe;
    }

    return 0;
}



static void clean_up_messaging(void);

int start_cgreen_messaging(int tag) {
    CgreenMessageQueue *tmp;
    int pipes[2];
    int pipe_result;

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

    pipe_result = cgreen_pipe_open(pipes);
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
    CgreenMessage *message;
    
    message = (CgreenMessage *) malloc(sizeof(CgreenMessage));
    if (message == NULL) {
      return;
    }
    memset(message, 0, sizeof(*message));
    message->type = queues[messaging].tag;
    message->result = result;
    cgreen_pipe_write(queues[messaging].writepipe, message, sizeof(CgreenMessage));
    // give the parent a chance to read so that failures are more likely to be output
    // before the child crashes
    sched_yield();

    free(message);
}

int receive_cgreen_message(int messaging) {
    ssize_t received;
    int result;
    CgreenMessage *message = (CgreenMessage *) malloc(sizeof(CgreenMessage));
    if (message == NULL) {
      return -1;
    }

    received = cgreen_pipe_read(queues[messaging].readpipe, message, sizeof(CgreenMessage));
    result = (received > 0 ? message->result : 0);
    free(message);
    return result;
}

static void clean_up_messaging(void) {
    int i;
    for (i = 0; i < queue_count; i++) {
        if (queues[i].owner == getpid()) {
            cgreen_pipe_close(queues[i].readpipe);
            cgreen_pipe_close(queues[i].writepipe);
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
