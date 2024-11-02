#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#include <time.h>

#include "utils.h"
#include "shmem.h"

const key_t SHMEM_ID = 888;

int main ()
{
    sem_unlink (PARENT_SEMAPHORE_NAME);
    sem_unlink (CHILD_SEMAPHORE_NAME);

    sem_t* parent_sem = sem_open (PARENT_SEMAPHORE_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (parent_sem == SEM_FAILED)
    {
        perror ("parent parent_sem sem_open error ");
        return 1;
    }

    sem_t* child_sem = sem_open (CHILD_SEMAPHORE_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (child_sem == SEM_FAILED)
    {
        perror ("parent child_sem sem_open error ");
        return 1;
    }

    pid_t pid = fork ();
    if (pid == -1)
    {
        perror ("fork error ");
        return 1;
    }

    if (pid)
    {
        buf buffer = {};
        buf_ctor (&buffer, BUFFER_CAPACITY);

        size_t shmem_size = BUFFER_CAPACITY + sizeof(size_t);

        int shmem_id = shmget (SHMEM_ID, shmem_size, IPC_CREAT | 0666);
        if (shmem_id == -1)
        {
            perror ("shmget error ");
            return 1;
        }

        int input_d = protected_input_file_open (INPUT_FILENAME);
        if (input_d == -1) return 1;

        char* shmaddr = shmat (shmem_id, NULL, 0);
        if (shmaddr == (void*)-1)
        {
            perror ("Parent shmat ");
            return 1;
        }

        int msg_size = 0;
        while (read_from_file (&buffer, input_d))
        {
            sem_wait (parent_sem);

            memcpy (shmaddr, &(buffer.size), sizeof(size_t));
            memcpy (shmaddr + sizeof(size_t), buffer.buf, buffer.capacity);

            sem_post (child_sem);
        }

        sem_wait (parent_sem);
        *((size_t*)shmaddr) = 0;
        sem_post (child_sem);

        close (input_d);
        buf_dtor (&buffer);
        sem_close( parent_sem);

        int status = 0;
        waitpid (pid, &status, 0);
    }
    else
    {
        size_t shmem_size = BUFFER_CAPACITY;

        int shmem_id = shmget (SHMEM_ID, shmem_size, IPC_CREAT | 0666);
        if (shmem_id == -1)
        {
            perror ("Child shmget error ");
            return 1;
        }

        char* shmaddr = shmat(shmem_id, NULL, 0);
        if (shmaddr == (void*)-1)
        {
            perror ("Child shmat ");
            return 1;
        }

        int output_d = protected_output_file_open (OUTPUT_FILENAME);
        if (output_d == -1) return 1;

        clock_t proc_time = clock();
        while (1)
        {
            sem_wait(child_sem);

            if (*((size_t*)shmaddr) == 0) break;

            if (write (output_d, shmaddr + sizeof(size_t), *((size_t*)shmaddr)) < 0)
            {
                perror("write error ");
                return 1;
            }
            sem_post (parent_sem);
        }
        proc_time = clock() - proc_time;

        printf ("Receive time: %ld\n", proc_time);

        close (output_d);
        sem_close (child_sem);

        struct shmid_ds shmid_ds;
        if (shmctl (shmem_id, IPC_RMID, &shmid_ds) == -1)
        {
            perror("Child shmctl ");
            return 1;
        }
    }
}
