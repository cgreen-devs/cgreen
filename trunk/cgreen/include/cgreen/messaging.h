#ifndef MESSAGING_HEADER
#define MESSAGING_HEADER

#ifdef __cplusplus
  extern "C" {
#endif

#include <sys/msg.h>

int start_cgreen_messaging(int tag);
void send_cgreen_message(int messaging, int result);
int receive_cgreen_message(int messaging);

#ifdef __cplusplus
    }
#endif

#endif
