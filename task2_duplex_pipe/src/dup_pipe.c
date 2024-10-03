#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "dup_pipe.h"

struct duplex_pipe* constructor ()
{
    struct duplex_pipe* pipe_ = calloc (1, sizeof(struct duplex_pipe));
    if (pipe_ == NULL)
    {
        perror("calloc error");
        return NULL;
    }
    if (pipe(pipe_->first_fd) || pipe(pipe_->second_fd))
    {
        perror ("pipe error");
        free(pipe_);
        return NULL;
    }

    pipe_->data_buff = NULL;
    pipe_->size   = 0;
    pipe_->read_  = read_to_pipe;
    pipe_->white_ = write_to_pipe;

    return pipe_;
}

void destructor (struct duplex_pipe* pipe)
{
    close (pipe->second_fd[0]);
    close (pipe->second_fd[0]);
    close (pipe->first_fd[1]);
    close (pipe->first_fd[1]);

    free(pipe);
}

bool is_open (int fd)
{
    return (fcntl (fd, F_GETFD) == -1) ? false : true;
}

int read_to_pipe (struct duplex_pipe* pipe)
{
    int fd = is_open (pipe->first_fd[0]) ? pipe->first_fd[0] : pipe->second_fd[0];
    return read (fd, pipe->data_buff, pipe->size);
}

int write_to_pipe (struct duplex_pipe* pipe)
{
    int fd = is_open (pipe->first_fd[1]) ? pipe->first_fd[1] : pipe->second_fd[1];
    return write (fd, pipe->data_buff, pipe->size);
}
