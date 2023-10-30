#include <fstream>
#include "parser/Location.hpp"
#include "parser/Parser.hpp"
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
		std::cout << "webserv: the configuration file " << fileName << " syntax is ok" << std::endl;
		std::cout << "webserv: configuration file " << fileName << " test is successful" << std::endl;
		printAllData(*parser);
		delete parser;
	}
	catch (ParsingException& e)
	{
		std::cout << e.what() << std::endl;
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
