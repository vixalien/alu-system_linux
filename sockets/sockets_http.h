#ifndef __SOCKETS_HTTP_H__
#define __SOCKETS_HTTP_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8080
#define BACKLOG 10

/* Macro Definitions */
#define VERBOSE_ON 1
#define RESP_OK "HTTP/1.1 200 OK\r\n\r\n"
#define RESP_OK_LEN (sizeof(RESP_OK) - 1)

/* Function Declarations */
int create_socket(void);
int bind_socket(int socket_fd);
int accept_and_receive(int socket_fd);
int init_socket(void);
int accept_recv(int serv_fd, char *buffer, int verbose);

#endif /* __SOCKETS_HTTP_H__ */

