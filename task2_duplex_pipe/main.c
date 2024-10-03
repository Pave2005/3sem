#include <stdio.h>

#include "data_processing.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    perror("Incorrect number of arguments");
    return ARGC_NUMBER_ERROR;
  }

  exchange_data (argv + 1);
}
