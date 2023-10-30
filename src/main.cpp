#include <fstream>
#include "Parser/Location.hpp"
#include "Parser/Parser.hpp"
#include "Models/ServerModel.hpp"
#include <cctype>


void	printAllData(Parser& parser)
{
	std::vector<ServerModel> servers = parser.getServers();
	std::vector<ServerModel>::iterator b = servers.begin();
	std::vector<ServerModel>::iterator e = servers.end();
	int i = 0;
	while (b < e)
	{
		std::cout << "===================================== Server " << i++ << "====================================" << std::endl;
		std::vector<Data>::const_iterator begin = b->getAllData().begin();
		std::vector<Data>::const_iterator end = b->getAllData().end();
		while (begin < end)
		{
			std::cout << "Key = " << begin->getKey() << "\tValue = " << begin->getValue() << std::endl;
			begin++;
		}
		parser.printServerModel(*b);
		b++;
	}
}

void	testLeaks(char *fileName)
{
	try
	{
		Parser* parser = new Parser(fileName);
		std::cout << "\033[32mwebserv: the configuration file " << fileName << " syntax is ok" << std::endl;
		std::cout << "webserv: configuration file " << fileName << " test is successful\033[0m" << std::endl;
		printAllData(*parser);
		delete parser;
	}
	catch (ParsingException& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;
	}
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	testLeaks(av[1]);
//	system("leaks -q webServ");	
	//checkSyntax(data);

	return (0);
}
