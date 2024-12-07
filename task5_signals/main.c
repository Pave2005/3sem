#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/stat.h>

#include "config.h"
#include "funcs.h"

sem_t* receive_sem  = NULL;
sem_t* write_sem    = NULL;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Invalid argument count\n");
        return -1;
    }

    sem_unlink(RECEIVE_SEMAPHORE_NAME);
    sem_unlink(WRITE_SEMAPHORE_NAME);

    receive_sem = sem_open(RECEIVE_SEMAPHORE_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (receive_sem == SEM_FAILED)
    {
        perror("receive_sem sem_open error");
        return 1;
    }

    write_sem = sem_open(WRITE_SEMAPHORE_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (write_sem == SEM_FAILED)
    {
        perror("write_sem sem_open error");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        return -1;
    }

    if (pid == 0)
    {
        if (receive_info())
        {
            fprintf(stderr, "Error in proccess!\n");
            return -1;
        }
    }
    else
    {
        if (write_info(argv[1], pid))
        {
            fprintf(stderr, "Error in proccess!\n");
            return -1;
        }
    }
}
