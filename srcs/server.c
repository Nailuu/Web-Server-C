#include "server.h"
#include "error.h"

Server *init_server(int port, int backlog)
{
  Server *server = NULL;
  struct sockaddr_in socket_params;
  int socket_fd;

  // Create TCP Socket and return socket file descriptor
  verify((socket_fd = socket(AF_INET, SOCK_STREAM, 0)),
         "Socket creation", 0, NULL);

  socket_params.sin_family = AF_INET; // ipv4
  socket_params.sin_port = htons(port);
  socket_params.sin_addr.s_addr = INADDR_ANY;

  verify(bind(socket_fd, (struct sockaddr *)&socket_params, sizeof(socket_params)),
         "Socket binding", 0, NULL);

  // Open socket and listen to given port with the given backlog
  verify(listen(socket_fd, backlog),
         "Socket listening", 0, NULL);

  // Allocate server structure
  server = (Server*)malloc(sizeof(server));
  if (server == NULL)
    return (NULL);

  // Populate server structure
  server->socket = socket_fd;

  return (server);
}

void close_server(Server *server)
{
  close(server->socket);
  free(server);
}
