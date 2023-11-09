#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <map>
#include <unistd.h>
#include<vector>
#include <arpa/inet.h>
#include<vector>
#include <sys/poll.h>
#include <sys/select.h>
#include <cstring>
using namespace std;
class	ServerRun
{
	public :
		ServerRun();
		int Newsocket();
		void bindConection(int Port, int sreverfd);
		void listenSocket(int serverfd);
		~ServerRun();
	private :

};

void acceptRquist( std::vector<int> servers );
#endif
