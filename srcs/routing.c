#include "routing.h"

Route *init_route(char *key, char *value, int is_editable)
{
  Route *root = (Route*)malloc(sizeof(Route));
  if (root == NULL)
    return (NULL);

  root->key = key;
  root->value = value;
  root->is_editable = is_editable;
  root->rigth = NULL;
  root->left = NULL;

  return (root);
}

Route *add_route(Route *root, char *key, char *value, int is_editable)
{
  if (root == NULL)
    return (init_route(key, value, is_editable));

  if (strcmp(key, root->key) > 0)
    root->rigth = add_route(root->rigth, key, value, is_editable);
  else 
    root->left = add_route(root->left, key, value, is_editable);

  return (root);
}

Route *search_route(Route *root, char *key)
{
  if (root == NULL)
    return (NULL);

  if (strcmp(key, root->key) == 0)
    return (root);

  else if (strcmp(key, root->key) > 0)
    return (search_route(root->rigth, key));

  else
    return (search_route(root->left, key));
}

void clear_route(Route *root)
{
  free(root);
}
