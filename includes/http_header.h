#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char *get_http_request_url(char *request);
char *get_http_method(char *request);
char *get_http_header(int method);
void http_invalid_method(int client_socket, int br);
void http_too_big(int client_socket, int br);
void http_not_found(int client_socket, int br);
void http_internal_error(int client_socket, int br);
void http_unauthorized(int client_socket, int br);
void http_ok(int client_socket, int br);
char *get_body(char *header);

#endif
