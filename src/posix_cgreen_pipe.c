#include "cgreen/internal/cgreen_pipe.h"

#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

#ifndef O_NONBLOCK
#    error "Your platform does not support POSIX ASYNC pipe reads. Please report a bug to http://github.com/cgreen-devs/cgreen/issues"
     /* One way to do it the old way is: ioctl(fd, FIOBIO, &flags); */
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
    if (0 != fcntl(p, F_SETFL, O_NONBLOCK)) {
        fprintf(stderr, "could not set file status flag on read pipe\n");
        return -1;
    }

    return read(p, buf, count);
}

ssize_t cgreen_pipe_write(int p, const void *buf, size_t count)
{
    int pipe_write_result = write(p, buf, count);
    int status;
    if (pipe_write_result < 0) {
        if (errno == EWOULDBLOCK) {
            PANIC("Too many assertions within a single test.");
        } else if (errno != EPIPE) {
            PANIC("Error when reporting from test case process to reporter.");
        }
        raise(SIGPIPE);
        wait(&status); /* Safe-guarding against a signalhandler for SIGPIPE, which
                          incidentally the test case for pipe block need to have... */
    }
    return pipe_write_result;
}

/* vim: set ts=4 sw=4 et cindent: */
