#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
class	ServerRun
{
	public :
		ServerRun();
		void bindConection(int Port);
		void listenSocket();
		void acceptRquist();
		~ServerRun();
	private :
		int socketfd;
		int newfd;
		struct sockaddr_in addressSocketStruct;

};
#endif
