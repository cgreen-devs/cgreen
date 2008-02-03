#ifndef MESSAGING_HEADER
#define MESSAGING_HEADER

#include <sys/msg.h>

int start_messaging(int tag);
void send_message(int messaging, int result);
int receive_message(int messaging);

#endif
