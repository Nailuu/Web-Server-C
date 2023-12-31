#include "file.h"
#include "http_header.h"

void send_file(int client_socket, Route *file_route)
{
  char buffer[4096] = {0};
  int bytes_read;

  // Open file and return code 500 if error
  int file_fd = open(file_route->value, O_RDONLY);\
  if (file_fd < 0)
  {
    http_internal_error(client_socket, 0);
    return ;
  }

  // Send header code 200
  write(client_socket, get_http_header(200), strlen(get_http_header(200)));

  // Read file and write client socket with content
  while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0)
  {
    write(client_socket, buffer, bytes_read);
    printf("SENT %d BYTES\n", bytes_read);
  }

  close(client_socket);
}

void post_file(int client_socket, Route *file_route, char *new_data)
{ 
  int bytes_written;

  if (file_route->is_editable == 0)
  {
    http_unauthorized(client_socket, 0);
    return ;
  }

  // Open file and return code 500 if error
  int file_fd = open(file_route->value, O_WRONLY|O_TRUNC);\
  if (file_fd < 0)
  {
    http_internal_error(client_socket, 0);
    return ;
  }

  // Send header code 200
  write(client_socket, get_http_header(200), strlen(get_http_header(200)));

  // Write file with new data string
  if (new_data != NULL)
  {
    bytes_written = write(file_fd, new_data, strlen(new_data));
    if (bytes_written < 0)
    {
      http_internal_error(client_socket, 0);
      return ;
    }
    printf("MODIFIED WITH %d BYTES\n", bytes_written);
  }
  else
    printf("MODIFIED WITH 0 BYTES\n");

  http_ok(client_socket, 0);
}
