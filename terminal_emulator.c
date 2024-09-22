#include "terminal.h"

int parse_cmd (char* cmd, char* args[maxArgsCount])
{
    char divElems[] = {' ', '\n'};

    char* lexem = strtok (cmd, divElems);

    for (int indx = 0; lexem != NULL; lexem = strtok (NULL, divElems), ++indx)
        args[indx] = lexem;

    return 0;
}

pid_t protected_fork ()
{
    pid_t pid = fork ();
    if (pid == -1)
    {
        perror ("fork error");
        exit (EXIT_FAILURE);
    }
    return pid;
}

void process (char* cmd, int inputFd)
{
    char* pipePos = strchr (cmd, '|');

    while (pipePos != NULL)
    {
        *pipePos = '\0';
        char* w_cmd = cmd;
        char* r_cmd = ++pipePos;

        int pipefd[2] = {};

        protected_va_funcs (pipe, pipefd);
        int r_descriptor = pipefd[0];
        int w_descriptor = pipefd[1];

        pid_t w_pid = protected_fork ();

        if (w_pid == 0)
        {
            protected_va_funcs (dup2, w_descriptor, STDOUT_FILENO);
            if (inputFd != STDIN_FILENO)
                protected_va_funcs (dup2, inputFd, STDIN_FILENO);

            close (pipefd[0]);
            close (pipefd[1]);

            char* args[maxArgsCount] = {};

            parse_cmd (w_cmd, args);

            protected_va_funcs (execvp, args[0], args);
        }
        else
        {
            pid_t r_pid = protected_fork ();

            if (r_pid == 0)
            {
                close (pipefd[1]);

                cmd = r_cmd;
                inputFd = r_descriptor;
                pipePos = strchr (r_cmd, '|');
                continue;
            }
            else
            {
                close (pipefd[0]);
                close (pipefd[1]);

                int status = 0;
                waitpid (w_pid, &status, 0);
                waitpid (r_pid, &status, 0);
            }
        }
    }

    pid_t pid = protected_fork ();

    if (pid == 0)
    {
        protected_va_funcs (dup2, inputFd, STDIN_FILENO);
        close (inputFd);

        char* args[100] = {};

        parse_cmd (cmd, args);

        protected_va_funcs (execvp, args[0], args);
    }
    else
    {
        int status = 0;
        waitpid (pid, &status, 0);
    }
}

void run_cmd ()
{
	char cmd[1000] = {};

	fgets (cmd, sizeof(cmd), stdin);
	cmd[strcspn (cmd, "\n")] = '\0';

	process (cmd, STDIN_FILENO);
}

