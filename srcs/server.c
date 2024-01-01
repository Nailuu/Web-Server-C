#include "server.h"

Server *init_server(int port, int backlog)
{
  Server *server = NULL;
  struct sockaddr_in socket_params;
  int socket_fd;

  // Create TCP Socket and return socket file descriptor
  verify((socket_fd = socket(AF_INET, SOCK_STREAM, 0)),
         "Socket creation");

  socket_params.sin_family = AF_INET; // ipv4
  socket_params.sin_port = htons(port);
  socket_params.sin_addr.s_addr = INADDR_ANY;

  verify(bind(socket_fd, (struct sockaddr *)&socket_params, sizeof(socket_params)),
         "Socket binding");

  // Open socket and listen to given port with the given backlog
  verify(listen(socket_fd, backlog),
         "Socket listening");

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

void handle_connection(ThreadArgs *t_args)
{
  char buffer[8192] = {0};
  char *request_info, *content, *body, *method, *requested_page;
  Route *route, *file_route;
  int client_socket, bytes_read;

  // Recovering args from thread
  client_socket = t_args->client_socket;
  route = t_args->route;
  free(t_args);

  if (client_socket < 0)
    return ;

  // Read http request from client
  if ((bytes_read = read(client_socket, buffer, sizeof(buffer))) == 8192)
  {
    http_too_big(client_socket, 1);
    return ;
  }
  if (bytes_read < 0)
    return ;

  // Extract first line of request header and other into content 
  request_info = buffer;
  content = strchr(buffer, '\n');
  *content = '\0';
  content++;

  // Extract body from header
  body = get_body(content);

  // Extract requested file 
  requested_page = get_http_request_url(request_info);
  if (requested_page == NULL)
    return ;

  printf("-----------------------------\n");

  // Extract HTTP Method
  method = get_http_method(request_info);
	printf("METHOD: '%s'\n", method);
  printf("URL: '%s'\n", requested_page);

  // if http method is something else then get or post return invalid
  if (strcmp(method, "GET") != 0 && strcmp(method, "POST") != 0)
  {
    http_invalid_method(client_socket, 1);
    return ;
  }

  // Get file location in route list
  file_route = search_route(route, requested_page);
  // Return 404 if not route found
  if (file_route == NULL)
  {
    http_not_found(client_socket, 1);
    return ;
  }
  // Call function for GET or POST Method
  else
  {
    printf("PATH TO FILE: '%s'\n", file_route->value);

    // GET - Read then write to client socket the content of the file
    if (strcmp(method, "GET") == 0)
      send_file(client_socket, file_route);

    // POST - Truncate file content to request header content
    else if (strcmp(method, "POST") == 0)
      post_file(client_socket, file_route, body);

    printf("-----------------------------\n\n");
  }

  return ;
}
