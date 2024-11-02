#pragma once

#define INPUT_FILENAME  "input.txt"
#define OUTPUT_FILENAME "output.txt"

#define MSG_TYPE 1
#define BUFFER_CAPACITY LARGE_BUGGER_SIZE
#define MESSAGE_CAPACITY 8192

typedef struct _Massage
{
    long mtype;
    char mtext[MESSAGE_CAPACITY];
} msg;
