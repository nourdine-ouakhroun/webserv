
#include"Server.hpp"

int main(int ac, char **av)
{
	static_cast<void>(ac);
	Parser parser(av[1]);
	ServerData serv(parser.getServers());
	ServerRun Server(serv);
	Server.RunAllServers();
}




