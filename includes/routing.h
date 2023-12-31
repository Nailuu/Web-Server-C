#ifndef ROUTING_H
#define ROUTING_H

#include <stdlib.h>
#include <string.h>

typedef struct Route
{
  char *key;
  char *value;
  int is_editable;

  struct Route *rigth, *left;
} Route;

Route *add_route(Route *root, char *key, char *value, int is_editable);
Route *search_route(Route *root, char *key);
void clear_route(Route *root);

#endif
