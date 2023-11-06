#ifndef	SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include "../Utils/String.hpp"
#include <sys/poll.h>

class Server
{
	std::vector<struct pollfd> pollFds;
	struct	sockaddr_in	socketData;
	socklen_t	socketLen;
	unsigned short	port;
	static const int errorNumber = -202;

	public :
		Server( void );
		Server(const unsigned short &_port);
		Server(const Server& copy);
		~Server( void ) throw();
		Server&	operator=(const Server& target);
		bool	createNewSocket(unsigned short port);
		int		waitingRequest( void );
		int		getAvailabeFD( void );
		int		accept(int targetSocket);
		String	recieve(int socket);
		int		send(int socket, String response);
};

#endif
