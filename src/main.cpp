#include <fstream>
#include "Parser/Location.hpp"
#include "Parser/Parser.hpp"
#include "Models/ServerModel.hpp"
#include "Server/Server.hpp"
#include "Utils/ServerData.hpp"
#include <cctype>
#include <unistd.h>


void	printAllData(Parser& parser)
{
/*	std::vector<ServerModel> servers = parser.getServers();
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
	}*/
	ServerData servers(parser.getServers());
	try
	{
		ServerModel smodel = servers.getServer("mehdi.com");	
		ServerModel::printServerModelInfo(smodel);
	}
	catch (...)
	{
		std::cout << "I can't found the exact server." << std::endl;
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
	try
	{
		int pid = fork();
		if (!pid)
		{
			Server server(8090);
			while (1)
			{
				int newSocket = server.accept();
				String header = server.recieve(newSocket);
				std::cout << header << std::endl << std::endl;
				server.send(newSocket, "HTTP/1.1 200 OK\r\n\r\n<h1>hello world</h1>");
				close(newSocket);
			}
		}
		wait(0);
	}
	catch (...)
	{
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
	int pid = getpid();
	String str("lsof -p ");
	str.append(std::to_string(pid));
	system("leaks -q webServ");	
	system(str.c_str());
	//checkSyntax(data);

	return (0);
}
