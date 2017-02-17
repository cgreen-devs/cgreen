#include <cgreen/messaging.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stack>
#include <vector>
#include <typeinfo>

namespace cgreen {
    extern "C" {

#define message_content_size(Type) (sizeof(Type) - sizeof(long))

struct CgreenMessage {
public:
    long type;
    int result;
    CgreenMessage() : type(0), result(0) {}
    CgreenMessage(const CgreenMessage& other) : type(other.type), result(other.result) {}
    CgreenMessage& operator=(const CgreenMessage& other) {
        if (this == &other) { return *this; }
        this->type = other.type;
        this->result = other.result;
        return *this;
    }
};

class CgreenMessageQueue {
public:
    CgreenMessageQueue(int tag_) : messages(), owner(0), tag(tag_) {}
    std::stack<CgreenMessage*> messages;
    pid_t owner;
    int tag;
};

static std::vector<CgreenMessageQueue> queues;
static int queue_count = 0;

static void clean_up_messaging(void);

int start_cgreen_messaging(int tag) {
    if (queue_count == 0) {
        int atexit_result = atexit(&clean_up_messaging);

        if (atexit_result != 0) {
            fprintf(stderr, "could not register clean up code\n");
            return -1;
        }
    }
    queue_count++;
    queues.push_back(CgreenMessageQueue(tag));
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
    message = queues[messaging].messages.top();
    fflush(stdout);
    queues[messaging].messages.pop();
    int result = message->result;
    delete message;
    return result;
}

static void clean_up_messaging() {
    queue_count = 0;
}
}
} // namespace cgreen

/* vim: set ts=4 sw=4 et cindent: */
