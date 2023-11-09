#include "Parser.hpp"
#include "Server.hpp"
#include "ServerData.hpp"
#include <unistd.h>

void	runServer(Server& server)
{
	int res, fd, newsocket;
	String	header;

	while (1)
	{
		res = server.waitingRequest();
		if (!res)
			continue ;
		if (res < 0)
			throw (std::exception());
		fd = server.getAvailabeFD();
		if (fd < 0)
			throw (std::exception());	
		newsocket = server.accept(fd);
		if (newsocket < 0)
			continue ;
		header = server.recieve(newsocket);
		if (header.empty() == true)
			break ;
		// Logger::debug(std::cout, header, "");
		Parser::parseHeader(header);
		
		if (server.send(newsocket, "HTTP/1.1 200 ok\r\n\r\n<h1>hello world</h1>") == -1)
			Logger::error(std::cerr, "Send Failed.", "");
		close(newsocket);
		header.clear();
	}
}

void	createServer(const ServerModel& serv)
{
	std::vector<Data> data = serv.getData("listen");
	std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
	Server server;
	while (ibegin < iend)
	{
		unsigned short port = (unsigned short)strtol(ibegin->getValue().c_str(), NULL, 10);
		Logger::debug(std::cout, "Port : ", port);
		if (server.createNewSocket(port) == false)
			std::cout << "can not open the port : " << port << std::endl;
		ibegin++;
	}
	runServer(server);
}

void	test(const Location& loca)
{
	std::vector<Data> locationData = loca.getAllData();
	String str("");
	for (int i = 0; i < (int)locationData.size(); i++)
		Data::printData(locationData.at((std::vector<Data>::size_type)i), str);
	Logger::debug(std::cerr, "hello form ", "test.");
}

void	printAllData(__unused Parser& parser)
{
	ServerData servers(parser.getServers());
	try
	{
		ServerModel smodel = servers.getDefaultServer();
		ServerModel::printServerModelInfo(smodel);
		createServer(smodel);
	}
	catch (std::exception& e)
	{
		Logger::error(std::cerr, "I can't found the exact server, Reason => ", e.what());
	}
}

void	testLeaks(char *fileName)
{
	try
	{
		Parser parser(fileName);
		String str("the configuration file");
		str.append(fileName);
		Logger::success(std::cout, str, " syntax is ok.");
		Logger::success(std::cout, str, " test is successfuli.");
		printAllData(parser);
	}
	catch (ParsingException& e)
	{
		Logger::error(std::cerr, e.what(), "");
	}
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		Logger::error(std::cerr, "Invalid argument", ".");
		return (1);
	}
	testLeaks(av[1]);
	// system("leaks -q webServ");
	return (0);
}
