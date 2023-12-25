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

int main(void)
{
	// Create TCP Socket and return file descriptor
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror("[ERROR] Socket creation");
		exit(EXIT_FAILURE);
	}

	// Define socket port and ip protocol (ipv4)
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(8080);
	socket_address.sin_addr.s_addr = INADDR_ANY;

	
	if (bind(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
	{
		perror("[ERROR] Bind socket");
		exit(EXIT_FAILURE);
	}

	// Listen to socket port with a backlog of 25
	if (listen(socket_fd, 25) < 0)
	{
		perror("[ERROR] Listen error");
		exit(EXIT_FAILURE);
	}

	char buffer[256] = {0};
	int client_socket_fd, requested_fd;
	char path[256] = {0};
	char header[2048];
	char header404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\n404 Not Found";
	char file[2048] = {0};
	while (1)
	{
		// Accept incoming connection
		client_socket_fd = accept(socket_fd, 0, 0);	
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
		printf("REQUEST: %s\n", buffer);
		*strchr(path, ' ') = '\0';
		printf("PATH: %s\n", path);

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
			if (read(requested_fd, file, sizeof(file)) < 0)
			{
				perror("[ERROR] Reading requested file");
				exit(EXIT_FAILURE);
			}
			printf("%s\n", file);
			strcat(header, file);
			close(requested_fd);
			send(client_socket_fd, header, sizeof(header), 0);
			printf("-------------------\n");
			printf("%s\n", header);
			printf("-------------------\n");
		}
		else
		{
			printf("------------------\n");
			printf("%s\n", header404);
			printf("------------------\n");
			send(client_socket_fd, header404, sizeof(header404), 0);
		}
		close(client_socket_fd);
	}
	return (0);
}
