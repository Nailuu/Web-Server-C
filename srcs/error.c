#include "error.h"
#include "server.h"

void verify(int f, char *msg, int close, Server *server)
{
  if (f < 0 && server != NULL && close == 1)
    close_server(server);
  if (f < 0)
  {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}
