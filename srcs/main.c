#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "server.h"

int main(void)
{
  Server *server = init_server(8080, 5);	

	char buffer[256] = {0};
	int client_socket_fd, requested_fd, bytes_read;
	char path[256] = {0};
	char header[2048];
	char header404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\n404 Not Found";
	char file[4096] = {0};
	while (1)
	{
		// Accept incoming connection
		printf("Waiting for incoming connection...\n");
		client_socket_fd = accept(server->socket, 0, 0);	
		printf("\n----------------------------\n");
		if (client_socket_fd < 0)
		{
			perror("[ERROR] Accept socket");
			exit(EXIT_FAILURE);
		}

		// Read client socket
		read(client_socket_fd, buffer, sizeof(buffer) - 1);

		// HTTP Request type and requested file 
		strcpy(path, strchr(buffer, ' ') + 2);
		*strchr(buffer, ' ') = '\0';
		printf("REQUEST TYPE: '%s'\n", buffer);
		*strchr(path, ' ') = '\0';
		printf("REQUESTED PATH: '%s'\n\n", path);

		// Send default data to client
		if (strlen(path) == 0)
		{
			memset(path, '\0', sizeof(path));
			strcat(path, "index.html");
		}
		if ((requested_fd = open(path, O_RDONLY)) >= 0)
		{
			memset(header, '\0', sizeof(header));
			memset(file, '\0', sizeof(file));
			strcat(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
			write(client_socket_fd, header, strlen(header));
			bytes_read = read(requested_fd, file, sizeof(file));
			while (bytes_read > 0)
			{
				if (write(client_socket_fd, file, bytes_read) < 0)
				{
					perror("[ERROR] Write to client socket");
					exit(EXIT_FAILURE);
				}
				printf("SENDING %d BYTES\n", bytes_read);
				bytes_read = read(requested_fd, file, sizeof(file));
			}
			if (bytes_read < 0)
			{
				perror("[ERROR] Reading requested file");
				exit(EXIT_FAILURE);
			}
			strcat(header, file);
			close(requested_fd);
		}
		else
			write(client_socket_fd, header404, sizeof(header404));
		printf("----------------------------\n\n");
		close(client_socket_fd);
	}
  close_server(server);
	return (0);
}
