#ifdef WIN32

#include "cgreen/internal/cgreen_pipe.h"
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <wincompat.h>


#ifdef __cplusplus
namespace cgreen {
#endif


ssize_t cgreen_pipe_read(int p, void *buf, size_t count)
{
    DWORD bytesRead;

    ReadFile((HANDLE)p, buf, (DWORD)count, &bytesRead, NULL);

    return bytesRead;
}

ssize_t cgreen_pipe_write(int p, const void *buf, size_t count)
{
    DWORD bytesWritten;

    WriteFile((HANDLE)p, buf, (DWORD)count, &bytesWritten, NULL);

    if (bytesWritten != count)
    {
        printf("failed to write %d bytes - error = %d\n",count,GetLastError());
    }

    return bytesWritten;
}

int cgreen_pipe_open(int pipes[2])
{
    SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    char handleString[256];
    DWORD result;

    //figure out if we are a child process, and if so, use the handle provided by our parent
    result = GetEnvironmentVariableA(CGREEN_READ_HANDLE,handleString,sizeof(handleString));

    if (result) //we are a child process
    {
        pipes[0] = (int)atoi(handleString);
        result = GetEnvironmentVariableA(CGREEN_WRITE_HANDLE,handleString,sizeof(handleString));
        pipes[1] = (int)atoi(handleString);

        //now that we have retrieved these handles, clear the environment variables so
        //that subsequent calls to this function will create new pipes
        //(This is needed for unit tests "will_report_beginning_and_successful_finishing_of_test"
        //and "will_report_failing_of_test_only_once")
        SetEnvironmentVariableA(CGREEN_READ_HANDLE,NULL);
        SetEnvironmentVariableA(CGREEN_WRITE_HANDLE,NULL);
    }
    else
    {
        HANDLE readpipe,writepipe;
        DWORD mode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
        const DWORD DEFAULT_BUFFER_SIZE = 0;

        if (!CreatePipe(&readpipe, &writepipe, &saAttr, DEFAULT_BUFFER_SIZE))
            return -1;

        //turn on NOWAIT
        if (!SetNamedPipeHandleState(readpipe,&mode,NULL,NULL))
        {
            fprintf(stderr, "could not set file status flag on read pipe\n");
            return -1;
        }

        pipes[0] = (int)readpipe;
        pipes[1] = (int)writepipe;
    }

    return 0;
}

void cgreen_pipe_close(int p)
{
    CloseHandle((HANDLE)p);
}


#ifdef __cplusplus
} // namespace cgreen
#endif

#endif
/* vim: set ts=4 sw=4 et cindent: */
