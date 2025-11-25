#include "sockets_http.h"

#define RESPONSE "HTTP/1.1 200 OK\r\n\r\n"

/**
 * print_path_and_values - prints path of request
 *
 * @buffer: HTTP request to print from
 */
void print_path_and_values(char *buffer)
{
	char *saveptr, *value, *query;

	strtok_r(buffer, " \t\r\n", &saveptr);
	printf("Path: %s\n", strtok_r(NULL, "?", &saveptr));
	while (1)
	{
		value = strtok_r(NULL, "=", &saveptr);
		query = strtok_r(NULL, "& ", &saveptr);
		if (query == NULL)
			break;
		printf("Query: \"%s\" -> \"%s\"\n", value, query);
	}
}

/**
 * accept_and_print - accepts entering connection, prints
 *
 * @server_fd: fd of server socket
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int accept_and_print(int server_fd)
{
	int client_fd;
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	char buffer[BUFSIZ];
	int bytes_received;

	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr *) &client,
			(socklen_t *) &client_len);
		if (client_fd < 0)
		{
			perror("accept");
			exit(1);
		}
		printf("Client connected: %s\n", inet_ntoa(client.sin_addr));
		memset(&*buffer, 0, BUFSIZ);
		bytes_received = recv(client_fd, buffer, BUFSIZ, 0);
		if (bytes_received < 0)
		{
			perror("recv");
			exit(1);
		}
		if (bytes_received > 0)
			printf("Raw request: \"%s\"\n", buffer);
		print_path_and_values(buffer);
		send(client_fd, RESPONSE, strlen(RESPONSE), 0);
		close(client_fd);
	}
	return (EXIT_SUCCESS);
}

/**
 * main - opens IPv4/TCP socket, listens to traffic on port 8080
 *
 *        accepts entering connection
 *        prints IP address of connection
 *        waits for oncoming message from connected client
 *        prints the full received HTTP request
 *        prints break-down of the first line of the received HTTP request
 *        sends back response to connected client (HTTP 200 OK)
 *        closes connection with client
 *        waits for next connection
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 *
 */
int main(void)
{
	int server_fd;

	setbuf(stdout, NULL);
	server_fd = create_socket();
	if (server_fd == EXIT_FAILURE)
		return (EXIT_FAILURE);

	accept_and_print(server_fd);

	close(server_fd);
	return (EXIT_SUCCESS);
}
