#ifndef THREADS_H
#define THREADS_H

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "routing.h"

typedef struct ThreadArgs
{
  int client_socket;
  Route *route;
} ThreadArgs;

typedef struct ThreadsQueue
{
  ThreadArgs *args;
  struct ThreadsQueue *next;
} ThreadsQueue;

ThreadArgs *init_thread_args(Route *route, int client_socket);
pthread_t *init_thread_pool(int size);
void enqueue_thread(ThreadArgs *t_args);
ThreadArgs *dequeue_thread(void);

#endif
