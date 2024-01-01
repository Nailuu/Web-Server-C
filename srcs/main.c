#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "server.h"
#include "routing.h"

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int)
{
  keep_running = 0;
}

int main(void)
{
  Server *server;
  Route *route;
  int client_socket;

  // Handle SIGINT and close program properly on CTRL+C
  struct sigaction a;
  a.sa_handler = sig_handler;
  a.sa_flags = 0;
  sigemptyset(&a.sa_mask);
  sigaction(SIGINT, &a, NULL);

  // Creating server structure with socket and given port/backlog
  server = init_server(8080, 5);	
  if (server == NULL)
    return (0);

  // Adding route to route binary tree list
  route = add_route(NULL, "/", "files/index.html", 0);
  add_route(route, "/favicon.ico", "files/favicon.ico", 0);
  add_route(route, "/about", "files/about.html", 1);
  add_route(route, "/about/map", "files/map.txt", 0);

  // Loop to handle client connection
	while (keep_running)
	{
		// Accept incoming connection
		printf("Waiting for incoming connection...\n\n");
		client_socket = accept(server->socket, 0, 0);

    // Function to handle connection 
    if (handle_connection(client_socket, route) == 0)
    {
      fprintf(stderr, "\nError while handling connection, closing server.\n");
      break;
    }
  }

  clear_route(route);
  close_server(server);
	return (0);
}
