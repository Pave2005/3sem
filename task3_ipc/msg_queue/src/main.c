#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#include "utils.h"
#include "msg_queue.h"

const key_t MSG_ID = 150;

int main ()
{
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

        int msg_d = msgget (MSG_ID, IPC_CREAT | 0666);
        if (msg_d == -1)
        {
            perror ("parent msgget error ");
            return 1;
        }

        printf ("MSG_D: %d\n", msg_d);

        msg send_buf = {.mtype = MSG_TYPE,
                       .mtext = {}};

        int input_d = protected_input_file_open (INPUT_FILENAME);
        if (input_d == -1) return 1;

        int msg_size = 0;
        while (msg_size = read_from_file (&buffer, input_d))
        {
            if (msg_size == -1)
            {
                perror ("read error ");
                return 1;
            }

            size_t pos = 0;
            while (pos + MESSAGE_CAPACITY < buffer.size)
            {
                memcpy (send_buf.mtext, buffer.buf + pos, MESSAGE_CAPACITY);

                pos += MESSAGE_CAPACITY;

                if (msgsnd (msg_d, (void*)&send_buf, MESSAGE_CAPACITY, 0))
                {
                    perror ("msgsnd error ");
                    return 1;
                }
            }

            memcpy (send_buf.mtext, buffer.buf + pos, buffer.size - pos);
            if (msgsnd (msg_d, (void*)&send_buf, buffer.size - pos, 0))
            {
                perror ("msgsnd error ");
                return 1;
            }
        }

        if (msgsnd (msg_d, (void *) &send_buf, 0, 0))
        {
            perror("msgsnd error ");
            return 1;
        }

        close    (input_d);
        buf_dtor (&buffer);

        int status = 0;
        waitpid (pid, &status, 0);
    }
    else
    {
        int msg_d = msgget (MSG_ID, 0666);
        if (msg_d == -1)
        {
            perror ("child msgget error ");
            return 1;
        }

        printf ("MSG_D: %d\n", msg_d);

        msg read_buf = {};

        int output_d = protected_output_file_open (OUTPUT_FILENAME);
        if (output_d == -1) return 1;

        clock_t proc_time = clock();

        int msg_size = 0;
        while (msg_size = msgrcv (msg_d, &read_buf, MESSAGE_CAPACITY, MSG_TYPE, 0))
        {
            if (msg_size == -1)
            {
                perror ("msgrcv error ");
                return 1;
            }

            if (write (output_d, read_buf.mtext, msg_size) < 0)
            {
                perror ("write error ");
                return 1;
            }
        }

        proc_time = clock() - proc_time;

        printf ("Receive time: %ld\n", proc_time);

        close (output_d);
        struct msqid_ds qstatus;
        msgctl (msg_d, IPC_RMID, &qstatus);
    }
}
