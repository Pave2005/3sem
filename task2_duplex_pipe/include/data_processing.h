#ifndef PROCESSING_H
#define PROCESSING_H

#define size_of_buff (1024*1024)

enum errors
{
    ARGC_NUMBER_ERROR   = 1,
    PIPE_CREATURE_ERROR = 2,
    FORK_ERROR          = 3,
};

int exchange_data         (char **filenames);
int transfer_file_to_pipe (struct duplex_pipe* pipe, const char* filename);
int transfer_pipe_to_file (struct duplex_pipe* pipe, const char* filename);

#endif
