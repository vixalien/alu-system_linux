#include "sockets_http.h"

/**
 * bind_socket - assigns address with bind()
 *
 * @socket_fd: fd of created socket
 * Return: 0 or exit(1) on failure
 */
int bind_socket(int socket_fd)
{
	struct sockaddr_in rem;

	memset(&rem, 0, sizeof(rem));
	rem.sin_family = AF_INET;
	rem.sin_addr.s_addr = htonl(INADDR_ANY);
	rem.sin_port = htons(PORT);

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &rem, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	if (bind(socket_fd, (struct sockaddr *)&rem, sizeof(rem)) < 0)
	{
		perror("bind");
		exit(1);
	}
	return (0);
}


/**
 * create_socket - creates socket and binds to it
 *
 * Return: EXIT_SUCCESS or -1
 */
int create_socket(void)
{
	int server_fd;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}

	if (bind_socket(server_fd) < 0)
		return (EXIT_FAILURE);

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		return (EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);

	return (server_fd);
}
