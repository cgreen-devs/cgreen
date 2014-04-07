#include "cgreen/internal/cgreen_pipe.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#ifndef O_ASYNC
#  define O_ASYNC FASYNC
#  ifndef FASYNC
#    error "Your POSIX platform does not support ASYNC pipe reads. Please report a bug to cgreen-devel@lists.sf.net"
#  endif
#endif

#ifdef __cplusplus
namespace cgreen {
#endif


int cgreen_pipe_open(int pipes[2])
{
    int pipe_open_result;
    int pipe_nonblock_result;

    pipe_open_result = pipe(pipes);

    if (pipe_open_result != 0) {
        return pipe_open_result;
    }

    pipe_nonblock_result = fcntl(pipes[1], F_SETFL, O_NONBLOCK); 

    if (pipe_nonblock_result != 0) {
        return pipe_open_result;
    }

    return 0;
}

void cgreen_pipe_close(int p)
{
    close(p);
}

ssize_t cgreen_pipe_read(int p, void *buf, size_t count)
{
    if (0 != fcntl(p, F_SETFL, O_ASYNC)) {
        fprintf(stderr, "could not set file status flag on read pipe\n");
        return -1;
    }

    return read(p, buf, count);
}

ssize_t cgreen_pipe_write(int p, const void *buf, size_t count)
{
    int pipe_write_result = write(p, buf, count);
    if (pipe_write_result < 0) {
       if (errno == EWOULDBLOCK) {
           fprintf(stderr, "\tToo many assertions (> 8192) within a single test.\n");
        } else if (errno != EPIPE) {
           fprintf(stderr, "\tError reporting from test case process to reporter\n");
        }

        kill(getpid(), SIGPIPE);
    }

    return write(p,buf,count);
}


#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
