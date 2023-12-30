#ifndef ROUTING_H
#define ROUTING_H

#include <stdlib.h>
#include <string.h>

typedef struct Route
{
  char *key;
  char *value;

  struct Route *rigth, *left;
} Route;

Route *initRoute(char *key, char *value);
Route *addRoute(Route *root, char *key, char *value);
Route *searchRoute(Route *root, char *key);

#endif
