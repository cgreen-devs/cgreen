#ifndef MESSAGING_HEADER
#define MESSAGING_HEADER

#ifdef __cplusplus
namespace cgreen {
    extern "C" {
#endif

int start_cgreen_messaging(int tag);
void send_cgreen_message(int messaging, int result);
int receive_cgreen_message(int messaging);
int get_pipe_read_handle();
int get_pipe_write_handle();

#ifdef __cplusplus
    }
}
#endif

#endif
