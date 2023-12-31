#include "error.h"

void verify(int f, char *msg)
{
  if (f < 0)
  {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}
