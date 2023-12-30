#include "client.h"


int handle_connection(int client_socket, Server *server)
{
  char buffer[512] = {0};
  char *path;
  int type;

  char header404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\n404 Not Found";

  verify(client_socket, "Accept Socket", 1, server);

  // Read http request from client
  if (read(client_socket, buffer, sizeof(buffer)) == 512)
  {
    fprintf(stderr, "Request too big!");
    write(client_socket,
          "HTTP/1.1 400 Bad Request\r\n\r\n<h1 align=center>Request too big!</h1>", 70);
    close(client_socket);
    return (1);
  }

  // Print Request to console
  path = get_http_request_path(buffer);
  if (path == NULL)
  {
    close(client_socket);
    return (0);
  }

  type = get_http_request_type(buffer);
  if (type == 1)
	  printf("REQUEST TYPE: 'GET'\n");
  else if (type == 2)
    printf("REQUEST TYPE: 'POST'\n");
  else 
  {
    fprintf(stderr, "Invalid HTTP Method");
    write(client_socket,
          "HTTP/1.1 400 Bad Request\r\n\r\n<h1 align=center>Invalid HTTP Method</h1>", 73);
    close(client_socket);
    return (1);
  }

	printf("FILE: '%s'\n\n", path);
  free(path);


}
