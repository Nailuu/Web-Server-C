#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "server.h"
#include "routing.h"
#include "threads.h"

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int)
{
  keep_running = 0;
}

#define SERVER_PORT 8080
#define BACKLOG_SIZE 25
#define THREAD_POOL_SIZE 25

int main(void)
{
  // Handle SIGINT and close program properly on CTRL+C
  struct sigaction a;
  a.sa_handler = sig_handler;
  a.sa_flags = 0;
  sigemptyset(&a.sa_mask);
  sigaction(SIGINT, &a, NULL);

  // Creating server structure with socket and given port/backlog
  Server *server = init_server(SERVER_PORT, BACKLOG_SIZE);	
  if (server == NULL)
    return (0);

  // Adding route to route binary tree list
  Route *route = add_route(NULL, "/", "files/index.html", 0);
  add_route(route, "/favicon.ico", "files/favicon.ico", 0);
  add_route(route, "/about", "files/about.html", 1);
  add_route(route, "/about/map", "files/map.txt", 0);

  // Creating mutex to lock thread and create safe queue
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  
  // Initialize thread pool with given size
  pthread_t *pool = init_thread_pool(THREAD_POOL_SIZE, &mutex);
  if (pool == NULL)
    return (1);

    // Loop to handle client connection
	while (keep_running)
	{
		// Accept incoming connection
		printf("Waiting for incoming connection...\n\n");
		int client_socket = accept(server->socket, 0, 0);

    // Create new pending task in thread pool
    ThreadArgs *t_args = init_thread_args(route, client_socket);
    if (t_args == NULL)
      break;
    pthread_mutex_lock(&mutex);
    enqueue_thread(t_args);
    pthread_mutex_unlock(&mutex);
  }

  // Freeing everything
  printf("\nClosing server...\n");
  free(pool);
  clear_route(route);
  close_server(server);
	return (0);
}
