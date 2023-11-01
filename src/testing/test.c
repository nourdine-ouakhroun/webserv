#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main ( void )
{
	int	socketFd;
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0)
		return (1);
	printf("Socket Fd : %d\n", socketFd);
	int a;
	scanf("%d", &a);
	return (0);
}
