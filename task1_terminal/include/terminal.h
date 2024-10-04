#ifndef TERMINAL
#define TERMINAL

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/wait.h>

#define protected_va_funcs(func, ...)   \
        if(func(__VA_ARGS__) == -1)		\
	    {								\
		    perror(#func);		        \
		    exit(EXIT_FAILURE);			\
	    }

#define maxArgsCount 100
#define cmdLineSize  1000

void run_cmd ();
void process (char* cmd, int inFd);
int parse_cmd (char* cmd, char* args[maxArgsCount]);
pid_t protected_fork ();

#endif
