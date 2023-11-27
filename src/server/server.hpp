#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"



#define SA 		struct sockaddr
#define SA_IN 	struct sockaddr_in
#define PORT 8082
#define SIZE 2048

class	Server
{
	public :
		Server();
		~Server();

	private :
		int			servFd;
		int			clientFd;
		SA_IN		servAddr;
};
#endif
