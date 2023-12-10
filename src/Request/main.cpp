// #include "Server.hpp"
// #include "Parser.hpp"
// #include "Checker.hpp"
// #include "ServerData.hpp"



// int main(int ac, char *av[])
// {
// 	if (ac != 2)
// 		return (0);
// 	Parser parsing(av[1]);
// 	Checker	check(parsing.getServers());
// 	check.fullCheck();
// 	ServerData servers(parsing.getServers());
	
// 	Server	s;
// }

#include"ManageServers.hpp"

int main(int ac, char **av)
{
	// static_cast<void>(ac);
	if (ac != 2)
	{
		Logger::error(std::cerr, "invalid argements.", "");
		return (1);
	}
	Parser parser(av[1]);
	ServerData serv(parser.getServers());
	ManageServers Manageservers(serv);
	Manageservers.runAllServers();
	Manageservers.acceptConection();
}
