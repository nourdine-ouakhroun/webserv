#include "server.hpp"

int main()
{
	ServerRun Server;
	Server.bindConection(4041);
	Server.listenSocket();
	Server.acceptRquist();
	
}
