#include "Server.hpp"
#include "../Includes/Parser.hpp"
#include "../Includes/Checker.hpp"
#include "../Includes/ServerData.hpp"



int main(int ac, char *av[])
{
	if (ac != 2)
		return (0);
	Parser parsing(av[1]);
	Checker	check(parsing.getServers());
	check.fullCheck();
	ServerData servers(parsing.getServers());
	
	Server	s;
}