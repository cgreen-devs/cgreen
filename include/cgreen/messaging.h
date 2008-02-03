#ifndef MESSAGING_HEADER
#define MESSAGING_HEADER

#include <sys/msg.h>

typedef struct CgreenMessageQueue_ {
    int queue;
    pid_t owner;
    int tag;
} CgreenMessageQueue;

typedef struct CgreenMessage_ {
    long type;
    int result;
} CgreenMessage;

int start_messaging(int tag);
void send_message(int messaging, int result);
int receive_message(int messaging);

#endif
