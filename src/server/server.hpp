#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
class	ServerRun
{
	public :
		ServerRun();
		~ServerRun();
	private :
		int socketfd;
		int newfd;

};
#endif
