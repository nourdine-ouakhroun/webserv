#include "Parser.hpp"
#include "Server.hpp"
#include "ServerData.hpp"
#include <unistd.h>

void	runServerByPoll(Server& __unused server, __unused std::vector<int> port)
{
	while (true)
	{
		Poll	tmpPoll(server.fds);
		int pollReturn = tmpPoll.waitingRequest();
		if (pollReturn < 0)
			break ;
		if (pollReturn == 0)
			continue ;
		for (int i = 0; i < (int)tmpPoll.fdsSize(); i++)
		{
			int readyFd = tmpPoll.getReadyFd(i);
			if (readyFd > -1)
			{
				if (find(port.begin(), port.end(), readyFd) != port.end())
				{
					int newClient = server.accept(readyFd);
					if (newClient < 0)
						break ;
					server.fds.push_fd(newClient);
				}
				else
				{
					String header = server.recieve(readyFd);
					if (header.empty() == true)
						continue ;
					Logger::debug(std::cout, header, "");
					if (server.send(readyFd, "HTTP/1.1 200 ok\r\n\r\n<h1>hello world</h1>") == -1)
							Logger::error(std::cerr, "Send Failed.", "");
					close(readyFd);
					tmpPoll.erase_fd(readyFd);
					server.fds.erase_fd(readyFd);
					
				}
			}
		}
		
	}
}

void	createServer(const ServerModel& serv)
{
	std::vector<int> ports;
	std::vector<Data> data = serv.getData("listen");
	std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
	Server server;
	int newSocket;
	while (ibegin < iend)
	{
		unsigned short port = (unsigned short)strtol(ibegin->getValue().c_str(), NULL, 10);
		Logger::debug(std::cout, "Port : ", port);
		newSocket = server.createNewSocket(port);
		if (newSocket == -1)
			std::cout << "can not open the port : " << port << std::endl;
		ports.push_back(newSocket);
		ibegin++;
	}
	runServerByPoll(server, ports);
}

void	test(const Location& loca)
{
	std::vector<Data> locationData = loca.getAllData();
	String str("");
	for (int i = 0; i < (int)locationData.size(); i++)
		Data::printData(locationData.at((std::vector<Data>::size_type)i), str);
	Logger::debug(std::cerr, "hello form ", "test.");
}

void	printAllData(Parser& parser)
{
	ServerData servers(parser.getServers());
	try
	{
		ServerModel smodel = servers.getDefaultServer();
		// ServerModel::printServerModelInfo(smodel);
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






// void	runServerBySelect(Server& __unused server, std::vector<int> port)
// {
// 	Select	tmpSet(server.fds);
// 	while (true)
// 	{
// 		tmpSet = server.fds;
// 		int selectReturn = tmpSet.waitingRequest();
// 		if (!selectReturn)
// 			continue ;
// 		if (selectReturn == -1)
// 			break ;
// 		for(int i = 0; i < tmpSet.getNumberOfFds() + 1; i++)
// 		{
// 			if (tmpSet.fd_isset(i) == true)
// 			{
// 				if (find(port.begin(), port.end(), i) != port.end())
// 				{
// 					int newClient = server.accept(i);
// 					if (newClient < 0)
// 						break ;
// 					server.fds.fd_set(newClient);
// 					if (newClient > tmpSet.getNumberOfFds())
// 						tmpSet.setNumberOfFds(newClient);
// 					Logger::info(std::cerr, "new connection : ", newClient);
// 				}
// 				else
// 				{
// 					String header = server.recieve(i);
// 					Logger::debug(std::cout, header, "");
// 					if (server.send(i, "HTTP/1.1 200 ok\r\n\r\n<h1>hello world</h1>") == -1)
// 							Logger::error(std::cerr, "Send Failed.", "");
// 					close(i);
// 					server.fds.fd_clear(i);
					
// 				} // end of else
// 			} // end of if
// 		} // end of for
// 	} // end of while
// }

