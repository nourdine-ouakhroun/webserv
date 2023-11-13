#include "Server.hpp"

Server::Server( void )
{
	socketLen = sizeof(socketData);
}

int	Server::createNewSocket(unsigned short port)
{
	int opt = 1;
	int nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nSocket < 0)
		return (-1);
	fcntl(nSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int status = setsockopt(nSocket, SOL_SOCKET ,SO_REUSEADDR , &opt, sizeof(int));
    if(status < 0)
	{
       		std::cerr << "Couldn't set options" << std::endl;
		return (-1);
	}
	struct sockaddr_in socketData;
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = INADDR_ANY;
	if (bind(nSocket, (struct sockaddr *)&socketData, socketLen) < 0 || listen(nSocket, 5) < 0)
	{
		close(nSocket);
		nSocket = -1;
		return (-1);
	}
	fds.push_fd(nSocket);
	return (nSocket);
}

Server::Server(const unsigned short &_port)
{
	int opt = 1;
	socketLen = sizeof(socketData);
	port = _port;
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw (ServerException("socket faild."));
	fcntl(socketFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int status = setsockopt(socketFd, SOL_SOCKET ,SO_REUSEADDR , &opt, sizeof(int));
	if(status < 0)
	{
       		std::cerr << "Couldn't set options" << std::endl;
		throw (ServerException("setsockopt faild."));
	}
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = INADDR_ANY;
	if (bind(socketFd, (struct sockaddr *)&socketData, socketLen) < 0 || listen(socketFd, 5) < 0)
	{
		close(socketFd);
		throw (ServerException("bind faild."));
	}
	fds.push_fd(socketFd);
}

Server::Server(const Server& copy)
{
	*this = copy;
}

Server::~Server( void ) throw()
{
	// close(socketFd);
}

Server&	Server::operator=(const Server& target)
{
	if (this != &target)
	{
		fds = target.fds;
		socketData = target.socketData;
		socketLen = target.socketLen;
		port = target.port;
	}
	return (*this);
}

int 	Server::accept(int targetSocket)
{
	int newSocket = (int)::accept(
						targetSocket,
						(struct sockaddr *)&socketData, 
						(socklen_t *)&socketLen
					);
	if (newSocket < 0)
		return (errorNumber);
	fcntl(newSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
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
		if ((nBytes = (int)recv(socket, tmp, 99, 0)) < 0)
			break ;
		buffer.append(tmp);
		if (nBytes < 99)
			break ;
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
