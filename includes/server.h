#ifndef SERVER_H
#define SERVER_H

typedef struct Server
{
  int socket;
} Server;

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/stat.h>

#include "error.h"
#include "http_header.h"
#include "routing.h"
#include "threads.h"
#include "file.h"

Server *init_server(int port, int backlog);
void close_server(Server *server);
void handle_connection(ThreadArgs *t_args);

#endif
