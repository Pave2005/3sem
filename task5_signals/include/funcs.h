#pragma once

#define OUTPUT_FILENAME "output_file.txt"

int receive_info();
int write_info(const char* filename, pid_t pid);
