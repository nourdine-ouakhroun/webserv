#include <fstream>
#include "Parser/Location.hpp"
#include "Parser/Parser.hpp"
#include "Models/ServerModel.hpp"
#include "Server/Server.hpp"
#include "Utils/ServerData.hpp"
#include <cctype>
#include <unistd.h>

/*void	parseHeader(String& header)
{

}
*/
void	runServer(Server& server)
{
	while (1)
	{
		int res = server.waitingRequest();
		if (!res)
			continue ;
		if (res < 0)
			throw (std::exception());
		int fd = server.getAvailabeFD();
		if (fd < 0)
			throw (std::exception());	
		int newsocket = server.accept(fd);
		String header = server.recieve(newsocket);
		std::cout << header << std::endl << std::endl;
		server.send(newsocket, "http/1.1 200 ok\r\n\r\n<h1>hello world</h1>");
		close(newsocket);
	}
}

void	createServer(const ServerModel& serv)
{
	(void)serv;
	std::vector<Data> data = serv.getData("listen");
	std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
	Server server;
	while (ibegin < iend)
	{
		unsigned short port = (unsigned short)strtol(ibegin->getValue().c_str(), NULL, 10);
		std::cout << "Port : " << port << std::endl;
		server.createNewSocket(port);
		ibegin++;
	}
	runServer(server);
}

void	printAllData(Parser& parser)
{
	__unused ServerData servers(parser.getServers());
	try
	{
		ServerModel smodel = servers.getServer("mehdi.com");
		//ServerModel::printServerModelInfo(smodel);
		createServer(smodel);
	}
	catch ( ... )
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
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	testLeaks(av[1]);
	return (0);
}
