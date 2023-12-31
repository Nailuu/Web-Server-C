#ifndef FILE_H
#define FILE_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "routing.h"

void send_file(int client_socket, Route *file_route);
void post_file(int client_socket, Route *file_route, char *new_data);

#endif
