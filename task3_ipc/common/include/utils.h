#pragma once

#include <stdio.h>

#define SMALL_BUFFER_SIZE  8192
#define MEDIUM_BUFFER_SIZE 65536
#define LARGE_BUGGER_SIZE  1048576

typedef struct _Buffer
{
    char* buf;
    size_t capacity;
    size_t size;
} buf;

int buf_ctor       (buf* buffer, size_t capacity);
int buf_dtor       (buf* buffer);
int read_from_file (buf* buffer, int fd);
int write_to_file  (const buf* buffer, int fd);

int protected_output_file_open (const char* filename);
int protected_input_file_open  (const char* filename);
