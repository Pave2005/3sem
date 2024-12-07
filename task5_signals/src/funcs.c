#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ucontext.h>

#include "config.h"
#include "funcs.h"

static void handler(int signo, siginfo_t* info, void* context)
{
    extern sem_t* write_sem;
    extern sem_t* receive_sem;

    sem_wait(receive_sem);

    FILE* output_file = fopen(OUTPUT_FILENAME, "ab");
    fwrite(&(info->si_value), sizeof(char), 1, output_file);
    fclose(output_file);

    sem_post(write_sem);
}

int receive_info()
{
    extern sem_t* write_sem;
    extern sem_t* receive_sem;

    struct sigaction act = {};
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handler;

    if (sigaction(1, &act, NULL))
    {
        perror("sigaction");
        return -1;
    }

    FILE* output_file = fopen(OUTPUT_FILENAME, "w");
    fclose(output_file);

    sem_post(write_sem);

    while (1) {}

    return 0;
}

int write_info (const char* filename, pid_t pid)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        perror("fopen");
        return -1;
    }

    struct stat st_file = {};
    if (stat(filename, &st_file))
    {
        perror("stat");
        return -1;
    }

    char* buffer = (char*)calloc(st_file.st_size + 1, sizeof(char));
    if (!buffer)
    {
        perror("calloc");
        return -1;
    }

    if (st_file.st_size != fread(buffer, sizeof(char), st_file.st_size, file))
    {
        perror("fread");
        return -1;
    }

    extern sem_t* write_sem;
    extern sem_t* receive_sem;

    union sigval info = {};
    for (size_t i = 0; i < st_file.st_size; ++i)
    {
        info.sival_int = buffer[i];
        sem_wait(write_sem);
        if(sigqueue(pid, 1, info))
        {
            perror("sigqueue");
            return -1;
        }
        sem_post(receive_sem);
    }

    sem_wait(write_sem);
    kill(pid, SIGKILL);

    sem_close(write_sem);
    sem_close(receive_sem);

    return 0;
}
