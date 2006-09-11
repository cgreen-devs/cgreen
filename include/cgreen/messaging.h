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

Message *create_message_buffer();
void destroy_message_buffer(Message *message);
void *start_messaging(int tag);
void send_message(MessageQueue *messaging, int result);
int receive_message(MessageQueue *messaging);

#endif
