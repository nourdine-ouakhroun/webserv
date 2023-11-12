#include "Parser.hpp"
#include "Server.hpp"
#include "ServerData.hpp"
#include <unistd.h>


String	readFile(const String& path)
{
	String content;
	char	buffer[2];

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (content);
	while (1)
	{
		memset(buffer, 0, 2);
		ssize_t reader = read(fd, buffer, 1);
		content.append(buffer);
		if (reader != 1)
			break ;
	}
	close(fd);
	return (content);
}

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
		// String name(indexs.at(i).getValue());
		tmp.append(indexs.at(i));
		Logger::info(std::cout, tmp, "");
		value = readFile(tmp);
		if (value.length() != 0)
			return ;
	}
	value.append("<h1>404 Page Not Found.</h1>");
	
}

String	handler(const ServerModel& servModel, std::vector<Data> header)
{
	GlobalModel model(header);
	std::vector<Data> hosts = model.getData("Method");
	String tmp(hosts.begin()->getValue());
	String path(tmp.split()[1]);
	path.rightTrim("/").trim(" \t\r\n");
	String str;
	String	content;
	if (ServerModel::findLocationByPath(servModel.getLocation(), str, path, to_do, content) == false)
		content.append("<h1>404 Page Not Found.</h1>");
	return (content);
}

void	runServerByPoll(const ServerModel& serv, Server& __unused server, __unused std::vector<int> port)
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
						std::cout << header << std::endl;
					String content("HTTP/1.1 200 ok\r\n\r\n");
					content.append(handler(serv, Parser::parseHeader(header)));
					if (server.send(readyFd, content) == -1)
							Logger::error(std::cerr, "Send Failed.", "");
					close(readyFd);
					tmpPoll.erase_fd(readyFd);
					server.fds.erase_fd(readyFd);
				}
			}
		}
	}
}

Server	createServer(const ServerModel& serv)
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
		createServer(servers.getDefaultServer());
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