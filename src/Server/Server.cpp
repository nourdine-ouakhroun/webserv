#include "Server.hpp"

Server::Server( void )
{
	socketLen = sizeof(socketData);
}

int	Server::createNewSocket(String ip, unsigned short port)
{
	int opt = 1;
	int nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nSocket < 0)
		return (-1);
	fcntl(nSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int status = setsockopt(nSocket, SOL_SOCKET ,SO_REUSEADDR , &opt, sizeof(int));
    if(status < 0)
	{
       	std::cerr << "Couldn't set options." << std::endl;
		return (-1);
	}
	struct sockaddr_in socketData;
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = convertStringToBinary(ip);
	std::cout << "socketData.sin_addr.s_addr : " << socketData.sin_addr.s_addr << std::endl;
	if (bind(nSocket, (struct sockaddr *)&socketData, socketLen) < 0 || listen(nSocket, 5) < 0)
	{
		close(nSocket);
		nSocket = -1;
		exit(1);
		return (-1);
	}
	fds.push_fd(nSocket, socketData);
	return (nSocket);
}

Server::Server(String _ip, const unsigned short _port)
{
	int opt = 1;
	socketLen = sizeof(socketData);
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw (ServerException("socket faild."));
	fcntl(socketFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int status = setsockopt(socketFd, SOL_SOCKET ,SO_REUSEADDR , &opt, sizeof(int));
	if(status < 0)
	{
		Logger::error(std::cerr, "Couldn't set options", "");
		throw (ServerException("setsockopt faild."));
	}
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(_port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = convertStringToBinary(_ip);
	if (bind(socketFd, (struct sockaddr *)&socketData, socketLen) < 0 || listen(socketFd, 5) < 0)
	{
		close(socketFd);
		Logger::error(std::cerr, "Couldn't bind address to socket.", "");
		throw (ServerException("bind faild."));
	}
	fds.push_fd(socketFd, socketData);
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
	}
	return (*this);
}

int 	Server::accept(int targetSocket, struct sockaddr* clientAddress)
{
	int newSocket = (int)::accept(targetSocket, clientAddress, (socklen_t *)&socketLen);
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

ssize_t	Server::send(int socket, String&	response)
{
	ssize_t	totalBytes = 0;
	size_t responseLength = response.length();
	while (1)
	{
		ssize_t	nBit = ::send(socket, response.c_str() + totalBytes, responseLength - (size_t)totalBytes, 0);
		if (nBit < 0)
		{
			if (totalBytes == (ssize_t)response.length())
				return (totalBytes);
			return (-1);
		}
		totalBytes += nBit;
		if (totalBytes == (ssize_t)response.length())
			return (totalBytes);
	}
	return (totalBytes);
}
