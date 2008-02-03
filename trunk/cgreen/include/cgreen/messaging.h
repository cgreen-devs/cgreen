#ifndef MESSAGING_HEADER
#define MESSAGING_HEADER

#include <sys/msg.h>

typedef struct MessageQueue_ {
    int queue;
    pid_t owner;
    int tag;
} MessageQueue;

typedef struct Message_ {
    long type;
    int result;
} Message;

int start_messaging(int tag);
void send_message(int messaging, int result);
int receive_message(int messaging);

#endif
