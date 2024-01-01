#include "threads.h"
#include "server.h"

ThreadsQueue *head = NULL;
ThreadsQueue *tail = NULL;

ThreadArgs *init_thread_args(Route *route, int client_socket)
{
  ThreadArgs *t_args = (ThreadArgs*)malloc(sizeof(ThreadArgs));

  if (t_args == NULL)
    return (NULL);

  t_args->route = route;
  t_args->client_socket = client_socket;

  return (t_args);
}

void *handle_thread(void *arg)
{
  pthread_mutex_t *mutex = (pthread_mutex_t*)arg;
  // Infinite loop to check for pending task in thread pool
  while (true)
  {
    // If dequeue return not NULL = there is a task
    pthread_mutex_lock(mutex);
    ThreadArgs *t_args = dequeue_thread();
    pthread_mutex_unlock(mutex);
    if (t_args != NULL)
    {
      // Handle connection
      handle_connection(t_args);
    }
  }

  return (void*)arg;
}

pthread_t *init_thread_pool(int size, pthread_mutex_t *mutex)
{
  pthread_t *pool = (pthread_t*)malloc(sizeof(pthread_t) * size);
  if (pool == NULL)
    return (NULL);

  // Create a thread for each element of thread array with the handle function
  for (int i = 0; i < size; i++)
  {
    if (pthread_create(&pool[i], NULL, handle_thread, (void*)mutex) != 0)
    {
      free(pool);
      return (NULL);
    }
  }

  return (pool);
}

void enqueue_thread(ThreadArgs *t_args)
{
  ThreadsQueue *new = (ThreadsQueue*)malloc(sizeof(ThreadsQueue));
  
  if (new == NULL)
    return ;

  new->args = t_args;
  new->next = NULL;

  if (tail == NULL)
    head = new;
  else
    tail->next = new;

  tail = new;
}

ThreadArgs *dequeue_thread(void)
{
  if (head == NULL)
    return (NULL);
  else
  {
    ThreadArgs *args = head->args;
    ThreadsQueue *tmp = head;
    head = head->next;

    if (head == NULL)
      tail = NULL;

    free(tmp);
    return (args);
  }
}
