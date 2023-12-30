#include "routing.h"

Route *initRoute(char *key, char *value)
{
  Route *root = (Route*)malloc(sizeof(Route));
  if (root == NULL)
    return (NULL);

  root->key = key;
  root->value = value;
  root->right = NULL;
  root->left = NULL;

  return (root);
}

Route *addRoute(Route *root, char *key, char *value)
{
  if (root == NULL)
    return (initRoute(key, value));

  if (strcmp(key, root->key) > 0)
    root->right = addRoute(route->right, key, value);
  else 
    root->left = addRoute(route->left, key, value);

  return (root);
}

Route *searchRoute(Route *root, char *key)
{
  if (root == NULL)
    return (NULL);

  if (strcmp(key, root->key) == 0)
    return (root);

  else if (strcmp(key, root->key) > 0)
    return (searchRoute(root->right, key));

  else
    return (searchRoute(root->left, key));
}
