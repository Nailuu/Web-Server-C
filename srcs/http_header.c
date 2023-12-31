#include "http_header.h"

char *get_http_request_url(char *request)
{
  *strchr(strchr(request, ' ') + 1, ' ') = '\0';

  return (strchr(request, ' ') + 1);
}

char *get_http_method(char *request)
{
  *strchr(request, ' ') = '\0';
  
  return (request);
}

char *get_http_header(int method)
{
  switch(method)
  {
    case 200:
      return ("HTTP/1.1 200 OK\r\n\r\n");

    case 400:
      return ("HTTP/1.1 400 Bad Request\r\n\r\n");

    case 401:
      return ("HTTP/1.1 401 Unauthorized\r\n\r\n");

    case 404:
      return ("HTTP/1.1 404 Not Found\r\n\r\n");

    case 500:
      return ("HTTP/1.1 500 Internal Server Error\r\n\r\n");

    default:
      return ("HTTP/1.1 400 Bad Request\r\n\r\n");
  }
}

void http_invalid_method(int client_socket, int br)
{
  printf("Invalid HTTP Method\n");
  if (br == 1)
    printf("-----------------------------\n\n");
  write(client_socket, get_http_header(400), strlen(get_http_header(400)));
  write(client_socket,
        "<h1 align=center>Invalid HTTP Method</h1>", 41);
  close(client_socket);
}

void http_too_big(int client_socket, int br)
{
  printf("Request too big!\n");
  if (br == 1)
    printf("-----------------------------\n\n");
  write(client_socket, get_http_header(400), strlen(get_http_header(400)));
  write(client_socket,
        "<h1 align=center>Request too big!</h1>", 38);
  close(client_socket);
}

void http_not_found(int client_socket, int br)
{
  printf("404 Not Found\n");
  if (br == 1)
    printf("-----------------------------\n\n");
  write(client_socket, get_http_header(404), strlen(get_http_header(404)));
  write(client_socket,
        "<h1 align=center>Page does not exist</h1>", 41);
  close(client_socket);
}

void http_internal_error(int client_socket, int br)
{
  printf("500 Internal Error\n");
  if (br == 1)
    printf("-----------------------------\n\n");
  write(client_socket, get_http_header(500), strlen(get_http_header(500)));
  write(client_socket,
        "<h1 align=center>Internal Server Error</h1>", 43);
  close(client_socket);
}

void http_unauthorized(int client_socket, int br)
{
  printf("401 Unauthorized\n");
  if (br == 1)
    printf("-----------------------------\n\n");
  write(client_socket, get_http_header(401), strlen(get_http_header(401)));
  write(client_socket,
        "<h1 align=center>Unauthorized</h1>", 34);
  close(client_socket);
}

void http_ok(int client_socket, int br)
{
  //printf("200 OK\n");
  if (br == 1)
    printf("-----------------------------\n\n");
  write(client_socket, get_http_header(200), strlen(get_http_header(200)));
  /*write(client_socket,
        "<h1 align=center>Unauthorized</h1>", 34);*/
  close(client_socket);
}

char *get_body(char *header)
{
  if (*header == '\0')
    return (NULL);

  for (long unsigned int i = 0; i < strlen(header) - 3; i++)
  {
    if (header[i] == '\r' && header[i + 1] == '\n' &&
      header[i + 2] == '\r' && header[i + 3] == '\n')
      return (&header[i + 4]);
  }

  return (NULL);
}
