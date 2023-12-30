#ifndef ERROR_H
#define ERROR_H

#include "server.h"

#include <stdio.h>
#include <stdlib.h>

void verify(int f, char *msg, int close, Server *server);

#endif
