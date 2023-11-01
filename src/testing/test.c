#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef struct sockaddr t_socket;

int main ( void )
{
	int	socketFd;
	struct sockaddr_in soc, cli;
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0)
		return (1);
	
	bzero(&soc, sizeof(t_socket));
	soc.sin_family = AF_INET;
	soc.sin_port = htons(80);
	soc.sin_addr.s_addr = INADDR_ANY;

	if (bind(socketFd, (struct sockaddr *)&soc, sizeof(soc)) == -1)
		return (1);
	else
		printf("connection's work successfuly!\n");	
	listen(socketFd, 5);
	int len = sizeof(cli);
	bzero(&cli, len);
	write(socketFd, "mehdisalim", 10);
	int connFd = accept(socketFd, (struct sockaddr *)&soc, (unsigned int *)&len);
	if (connFd < 0)
		return (8);
	char buffer[10];
	read(connFd, buffer, 10);
	printf("text => %s\n", buffer);
	printf("Socket Fd : %d\n", socketFd);
	return (0);
}
