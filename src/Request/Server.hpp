#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "Request.hpp"
// #include "Response.hpp"



#define SA 		struct sockaddr
#define SA_IN 	struct sockaddr_in
#define PORT 8080
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