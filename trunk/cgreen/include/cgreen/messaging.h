#ifndef _MESSAGING_HEADER_
#define _MESSAGING_HEADER_

#include <sys/msg.h>

typedef struct _MessageQueue {
    int queue;
    pid_t owner;
    int tag;
} MessageQueue;

typedef struct _Message {
    long type;
    int result;
} Message;

int start_messaging(int tag);
void send_message(int messaging, int result);
int receive_message(int messaging);

#endif
