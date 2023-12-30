#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include <string.h>
#include <stdlib.h>

char *get_http_request_path(char *request);
int get_http_request_type(char *request);

#endif
