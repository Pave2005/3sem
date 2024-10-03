#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dup_pipe.h"
#include "data_processing.h"

int exchange_data (char** filesname)
{
    struct duplex_pipe* pipe = constructor ();
    if (!pipe)
    {
        perror("constuctor error");
        return PIPE_CREATURE_ERROR;
    }

    pid_t pid = fork ();
    if (pid == -1)
    {
        perror("fork error");
        destructor (pipe);
        return FORK_ERROR;
    }

    if (pid == 0)
    {
        close (pipe->first_fd[1]);
        close (pipe->second_fd[0]);

        transfer_pipe_to_file (pipe, filesname[1]);
    }
    else
    {
        close (pipe->first_fd[0]);
        close (pipe->second_fd[1]);

        transfer_file_to_pipe (pipe, filesname[0]);
        close (pipe->first_fd[1]);

        int status = 0;
        wait (&status);
    }

    destructor (pipe);

    return 0;
}

int transfer_pipe_to_file (struct duplex_pipe* pipe, const char* filename)
{
    pipe->size = size_of_buff;
    pipe->data_buff = calloc (1, size_of_buff);
    if (!pipe->data_buff)
    {
        perror ("calloc error");
        exit (EXIT_FAILURE);
    }

    int fd = open(filename, O_WRONLY | O_CREAT);
    if (fd == -1)
    {
        perror ("oppening error");
        free (pipe->data_buff);
        exit (EXIT_FAILURE);
    }

    int count = pipe->read_(pipe);
    while (count)
    {
        if (count == -1)
        {
            perror ("writing to file error");
            break;
        }

        write (fd, pipe->data_buff, count);
        count = pipe->read_(pipe);
    }

    free (pipe->data_buff);
    close (fd);

    return 0;
}

int transfer_file_to_pipe (struct duplex_pipe* pipe, const char* filename)
{
    pipe->data_buff = calloc (1, size_of_buff);
    if (!pipe->data_buff)
    {
        perror ("calloc error");
        exit (EXIT_FAILURE);
    }

    int fd = open (filename, O_RDONLY);
    if (fd == -1)
    {
        perror ("oppening error");
        free (pipe->data_buff);
        exit (EXIT_FAILURE);
    }

    int count = read (fd, pipe->data_buff, size_of_buff);
    while (count)
    {
        if (count == -1)
        {
            perror ("reading from file error");
            break;
        }

        pipe->size = count;
        if (pipe->white_(pipe) == -1)
        {
            perror ("writing to pipe error");
            break;
        }
        count = read (fd, pipe->data_buff, size_of_buff);
    }

    close (fd);
    free  (pipe->data_buff);

    return 0;
}
