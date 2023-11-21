#include "server.hpp"
#include "Parser.hpp"
#include "ServerData.hpp"
#include "Checker.hpp"

int main()
{
	Parser	parser("configurations/default.conf");
	Checker checker(parser.getServers());
	checker.fullCheck();
	ServerData servers(parser.getServers());
	std::vector<ServerModel> serv = servers.getAllServers();
	Server s;
}
