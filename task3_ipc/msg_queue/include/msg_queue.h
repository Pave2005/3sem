#pragma once

#define INPUT_FILENAME  "./input.txt"
#define OUTPUT_FILENAME "./output.txt"

#define MSG_TYPE 1
#define BUFFER_CAPACITY MEDIUM_BUFFER_SIZE
#define MESSAGE_CAPACITY 8192

struct msgbuf
{
    long mtype;
    char mtext[MESSAGE_CAPACITY];
};
