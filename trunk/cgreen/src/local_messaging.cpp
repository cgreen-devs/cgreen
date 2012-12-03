#include <cgreen/messaging.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stack>
#include <typeinfo>
#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

#define message_content_size(Type) (sizeof(Type) - sizeof(long))

struct CgreenMessage {
public:
    long type;
    int result;
    CgreenMessage() : type(0), result(0) { printf("ctor\n"); }
    CgreenMessage(const CgreenMessage& other) : type(other.type), result(other.result) {}
    CgreenMessage& operator=(const CgreenMessage& other) {
        if (this == &other) { return *this; }
        printf("operator=\n");
        this->type = other.type;
        this->result = other.result;
        printf("end operator=\n");
        return *this;
    }
};

typedef struct CgreenMessageQueue_ {
    std::stack<CgreenMessage*> messages;
    pid_t owner;
    int tag;
} CgreenMessageQueue;

static CgreenMessageQueue *queues = NULL;
static int queue_count = 0;

static void clean_up_messaging(void);

int start_cgreen_messaging(int tag) {
    CgreenMessageQueue *tmp;

    if (queue_count == 0) {
        int atexit_result = atexit(&clean_up_messaging);

        if (atexit_result != 0) {
        	fprintf(stderr, "could not register clean up code\n");
        	return -1;
        }
    }
    tmp = (CgreenMessageQueue *) realloc(queues, sizeof(CgreenMessageQueue) * ++queue_count);
    printf("queue_count: %d\n", queue_count);
    if (tmp == NULL) {
    	/* ignoring return value here, as the world is ending anyways */
        (void)atexit(&clean_up_messaging);
        return -1;
    }
    queues = tmp;
    queues[queue_count - 1].owner = getpid();
    queues[queue_count - 1].tag = tag;
    return queue_count - 1;
}

void send_cgreen_message(int messaging, int result) {
    CgreenMessage* message = new CgreenMessage;
    message->type = queues[messaging].tag;
    message->result = result;
    queues[messaging].messages.push(message);
    //printf("push type %d result %d\n", message.type, message.result);
}

int receive_cgreen_message(int messaging) {
    if (queues[messaging].messages.empty()) {
       return 0;
    }
    
    CgreenMessage *message;
    queues[messaging].messages.top();
    printf("pre-top\n");
    message = queues[messaging].messages.top();
    printf("post-top\n");
    fflush(stdout);
    queues[messaging].messages.pop();
    //printf("pop type %d result %d\n", message.type, message.result);
    int result = message->result;
    delete message;
    return result;
}

static void clean_up_messaging() {
    printf("clean_up\n");
    free(queues);
    queues = NULL;
    queue_count = 0;
}

#ifdef __cplusplus
}
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
