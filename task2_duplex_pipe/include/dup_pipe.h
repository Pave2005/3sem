#ifndef PIPE_H
#define PIPE_H

#include <stdio.h>
#include <stdbool.h>

struct duplex_pipe
{
    int size;
    char* data_buff;

    int first_fd[2];
    int second_fd[2];

    int (*read_)(struct duplex_pipe* pipe);
    int (*white_)(struct duplex_pipe* pipe);
};

bool is_open       (int fd);
int  read_to_pipe  (struct duplex_pipe* pipe);
int  write_to_pipe (struct duplex_pipe* pipe);
void destructor    (struct duplex_pipe* pipe);
struct duplex_pipe* constructor ();

#endif
