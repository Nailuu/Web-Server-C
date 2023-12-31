#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "routing.h"

int main(void)
{
  Server *server;
  Route *route;
  int client_socket;

  // Creating server structure with socket and given port/backlog
  server = init_server(8080, 5);	
  if (server == NULL)
    return (0);

  // Adding route to route binary tree list
  route = add_route(NULL, "/", "files/index.html", 0);
  add_route(route, "/favicon.ico", "files/favicon.ico", 0);
  add_route(route, "/about", "files/about.html", 1);
  add_route(route, "/about/map", "files/map.txt", 1);

  // Loop to handle client connection
	while (1)
	{
		// Accept incoming connection
		printf("Waiting for incoming connection...\n\n");
		client_socket = accept(server->socket, 0, 0);	

    // Function to handle connection 
    if (handle_connection(client_socket, route) == 0)
    {
      fprintf(stderr, "Error while handling connection\n");
      clear_route(route);
      close_server(server);
      exit(EXIT_FAILURE);
    }
  }

	return (0);
}
