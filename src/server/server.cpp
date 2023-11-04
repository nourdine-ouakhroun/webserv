#include "Server.hpp"

Server::Server( void )
{
	socketLen = sizeof(socketData);
}

bool	Server::createNewSocket(unsigned short port)
{
	int nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nSocket < 0)
		return (false);
	struct sockaddr_in socketData;
	bzero(&socketData, socketLen);
	socketData.sin_port = htons(port);
	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = INADDR_ANY;
	if (bind(nSocket, (struct sockaddr *)&socketData, socketLen) < 0 || listen(nSocket, 5) < 0)
		return (false);	
	struct pollfd pFd;
	pFd.fd = nSocket;
	pFd.events = POLLIN | POLLOUT;
	pollFds.push_back(pFd);
	return (true);
}

Server::Server(const unsigned short &_port)
{
	socketLen = sizeof(socketData);
	port = _port;
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
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
	struct pollfd pFd;
	pFd.fd = socketFd;
	pFd.events = POLLIN | POLLOUT;
	pollFds.push_back(pFd);
}

Server::Server(const Server& copy)
{
	*this = copy;
}

Server::~Server( void ) throw()
{
	//close(socketFd);
}

Server&	Server::operator=(const Server& target)
{
	if (this != &target)
	{
		pollFds = target.pollFds;
		socketData = target.socketData;
		socketLen = target.socketLen;
		port = target.port;
	}
	return (*this);
}

int		Server::waitingRequest( void )
{
	return (static_cast<int>(poll(&pollFds[0], (unsigned int)pollFds.size(), 50000)));
}

int		Server::getAvailabeFD( void )
{
	std::vector<struct pollfd>::iterator iterBegin = pollFds.begin();
	std::vector<struct pollfd>::iterator iterEnd = pollFds.end();
	while (iterBegin < iterEnd)
	{
		if ((iterBegin->revents & POLLIN) || (iterBegin->revents & POLLOUT))
			return (iterBegin->fd);
		iterBegin++;
	}
	return (-1);
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
		{
			exit(0);
			break ;
		}
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
