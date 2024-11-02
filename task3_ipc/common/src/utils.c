#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

int buf_ctor (buf* buffer, size_t capacity)
{
    buffer->buf = (char*)calloc (capacity, sizeof(char));
    if (!(buffer->buf))
    {
        perror ("calloc error\n");
        return 1;
    }

    buffer->capacity = capacity;
    buffer->size     = 0;

    return 0;
}

int buf_dtor (buf* buffer)
{
    free (buffer->buf);

    buffer->capacity = 0;
    buffer->size     = 0;

    return 0;
}

int read_from_file (buf* buffer, int fd)
{
    buffer->size = read (fd, buffer->buf, buffer->capacity);
    if (buffer->size == -1)
    {
        perror ("read error\n");
        return -1;
    }

    return buffer->size;
}

int write_to_file (const buf* buffer, int fd)
{
    int w_bytes = write (fd, buffer->buf, buffer->size);
    if (w_bytes == -1)
    {
        perror ("write error\n");
        return -1;
    }

    return w_bytes;
}

int protected_output_file_open (const char* filename)
{
    int fd = open (filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) perror ("open output error\n");

    return fd;
}

int protected_input_file_open (const char* filename)
{
    int fd = open (filename, O_RDONLY);
    if (fd == -1) perror("open input error\n");

    return fd;
}
