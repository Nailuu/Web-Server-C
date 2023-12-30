#include "http_header.h"

char *get_http_request_path(char *request)
{
  char tmp[512] = {0};
  char *path;

  *strchr(strchr(request, ' ') + 1, ' ') = '\0';
  strcpy(tmp, strchr(request, ' ') + 1);

  path = (char*)malloc(sizeof(char) * (strlen(tmp) + 1));
  if (path == NULL)
    return (NULL);

  strcpy(path, tmp);

  return (path);
}

int get_http_request_type(char *request)
{
  char tmp[512] = {0};

  *strchr(request, ' ') = '\0';
  
  switch (request)
  {
    case 'GET':
      return (1);
    
    case 'POST':
      return (2);

    default:
      return (0);
  }
}
