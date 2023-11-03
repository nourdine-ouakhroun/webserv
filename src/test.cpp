#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Models/ServerModel.hpp"

int	main( void )
{
	struct sockaddr_in socketData;
	int socketLen = sizeof(socketData);
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw (std::exception());
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(8090);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = INADDR_ANY;
	if (bind(socketFd, (struct sockaddr *)&socketData, (int)socketLen) < 0)
		return (3);
	if (listen(socketFd, 5) < 0)
		return  (4);
	while (1)
	{

		int newSocket = accept(socketFd, (struct sockaddr *)&socketData, (socklen_t *)&socketLen);
		if (newSocket < 0)
			break ;
		std::string	buffer;
		char	tmp[1024];
		bzero(tmp, 1024);
		if (recv(newSocket, tmp, 1023, 0) < 0)
				break ;
		buffer.append(tmp);
		std::cout << buffer << std::endl;
		std::string response("HTTP/1.1 200 OK\r\n\r\n<h1>hello world</h1>");
		send(newSocket, response.c_str(), response.length(), 0);
		close(newSocket);
	}
	return (0);
}
