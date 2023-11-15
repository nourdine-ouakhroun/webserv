#include "Parser.hpp"
#include "Server.hpp"


template <typename T>
void	to_do(const Location& loca, T& value)
{
	String file(loca.getData("root").at(0).getValue());
	file.append(loca.getPath());
			 /* |   check if the path ended with /   | */
	file.append((file.end() - 1)[0] == '/' ? "" : "/");
	std::vector<String> indexs = String(loca.getData("index").at(0).getValue()).split();
	for (size_t i = 0; i < indexs.size(); i++)
	{
		String tmp(file);
		tmp.append(indexs.at(i));
		Logger::info(std::cout, tmp, "");
		value = readFile(tmp);
		if (value.length() != 0)
			return ;
	}
	value.append(ERROR_404);
}


String	handler(ServerData& servers, std::vector<Data> header)
{
	GlobalModel model(header);
	std::vector<ServerModel>	servModel;
	String str;
	String	content;

	servModel = getServer(servers, header);
	if (servModel.empty() == true)
		servModel = servers.getAllServers();
	// exit(0);
	// String access_log = servModel.at(0).getData("access_log").at(0).getValue();
	// if (access_log.contains("main") == true)
	// {
	// 	std::ofstream accessLogFile(access_log.split()[0].trim(" \t\n\r"));
	// 	Logger::info(accessLogFile, "Hello World", " Test 1");
	// }
	// std::vector<Data> hosts = model.getData("Method");
	String path(String(model.getData("Method").begin()->getValue()).split()[1]);
	path.rightTrim("/").trim(" \t\r\n");
	if (ServerModel::findLocationByPath(servModel.at(0).getLocation(), str, path, to_do, content) == false)
		return (ERROR_404);
	return (content);
}

bool	requestHandler(const std::vector<int>& port, Server& server, ServerData& serv, int readyFd)
{
	if (readyFd > -1)
	{
		if (find(port.begin(), port.end(), readyFd) != port.end())
		{
			int newClient = server.accept(readyFd);
			if (newClient < 0)
				return (false);
			server.fds.push_fd(newClient);
		}
		else
		{
			String header = server.recieve(readyFd);
			if (header.empty() == true)
				return (true);
			std::cout << header << std::endl;
			String content("HTTP/1.1 200 ok\r\n\r\n");
			content.append(handler(serv, Parser::parseHeader(header)));
			if (server.send(readyFd, content) == -1)
				Logger::error(std::cerr, "Send Failed.", "");
			close(readyFd);
			server.fds.erase_fd(readyFd);
		}
	}
	return (true);
}

void	runServerByPoll(ServerData& serv, Server& server, std::vector<int> port)
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
			if (requestHandler(port, server, serv, tmpPoll.getReadyFd(i)) == false)
				break ;
	}
}

Server	createServer(ServerData& serv)
{
	Server server;
	std::vector<int> ports = openAllPorts(serv.getAllServers(), server);
	runServerByPoll(serv, server, ports);
	for (size_t i = 0; i < ports.size(); i++)
		close(ports.at(i));
	return (server);
}

void	start(Parser& parser)
{
	ServerData servers(parser.getServers());
	try
	{
		createServer(servers);
	}
	catch (std::exception& e)
	{
		Logger::error(std::cerr, "I can't found the exact server, Reason => ", e.what());
	}
}


/**
 * @brief	main function.
 */
int	main(int ac, char **av)
{
	if (ac < 2)
	{
		Logger::error(std::cerr, "Invalid argument", ".");
		return (1);
	}
	try
	{
		Parser parser(av[1]);
		start(parser);
	}
	catch (ParsingException& e)
	{
		Logger::error(std::cerr, e.what(), "");
	}
	return (0);
}

/**
 * String str("the configuration file");
 * str.append(av[1]);
 * Logger::success(std::cout, str, " syntax is ok.");
 * Logger::success(std::cout, str, " test is successfuli.");
*/
