#include "Server.hpp"

Server::Server( void )
{
	socketLen = sizeof(socketData);
	port = 8778;
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw (std::exception());
	bzero(&socketData, (size_t)socketLen);
	socketData.sin_port = htons(port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = INADDR_ANY;
	if (bind(socketFd, (struct sockaddr *)&socketData, socketLen) < 0 || listen(socketFd, 5) < 0)
	{
		close(socketFd);
		throw (std::exception());
	}
}

Server::Server(const unsigned short &_port)
{
	socketLen = sizeof(socketData);
	port = _port;
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw (std::exception());
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = INADDR_ANY;
	if (bind(socketFd, (struct sockaddr *)&socketData, socketLen) < 0 || listen(socketFd, 5) < 0)
	{
		close(socketFd);
		throw (std::exception());
	}
}

Server::Server(const Server& copy)
{
	*this = copy;
}

Server::~Server( void ) throw()
{
	close(socketFd);
}

Server&	Server::operator=(const Server& target)
{
	if (this != &target)
	{
		socketFd = target.socketFd;
		socketData = target.socketData;
		socketLen = target.socketLen;
		port = target.port;
	}
	return (*this);
}

int 	Server::accept( void )
{
	int newSocket = (int)::accept(
						socketFd,
						(struct sockaddr *)&socketData, 
						(socklen_t *)&socketLen
					);
	if (newSocket < 0)
		return (errorNumber);
	return (newSocket);
}


String	Server::recieve(int socket)
{
	String	buffer;
	char	tmp[100];

	while (1)
	{
		bzero(tmp, 100);
		int nBytes;
		if ((nBytes = (int)::recv(socket, tmp, 99, 0)) < 0)
			break ;
		if (nBytes != 99)
			break ;
		buffer.append(tmp);
	}
	return (buffer);
}

int	Server::send(int socket, String response)
{

	int	nBit = (int)::send(socket, response.c_str(), response.length(), 0);
	if (nBit < 0)
		return (errorNumber);
	return (nBit);
}
