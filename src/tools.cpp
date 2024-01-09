#include "Server.hpp"
#include "Logger.hpp"

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

unsigned short	getPort(String	value)
{
	unsigned short port = 80;
	String::size_type pos = value.find_last_of(':');
	if (pos != String::npos)
		port = (unsigned short)std::strtol(value.substr(pos + 1).c_str(), NULL, 10);
	return port;
}

std::vector<int>	openAllPorts(const std::vector<ServerPattern>& serversInfo, Server& server)
{
	std::vector<int> ports;
	std::vector<String> tmpInfo;
	int newSocket;
	for (size_t i = 0; i < serversInfo.size(); i++)
	{
		std::vector<Data> data = serversInfo[i].getData("listen");
		for (size_t i = 0; i < data.size(); i++)
		{
			if (std::find(tmpInfo.begin(), tmpInfo.end(), data[i].getValue()) != tmpInfo.end())
				continue ;
			tmpInfo.push_back(data[i].getValue());
			std::vector<String> listen = data[i].getValue().split(':');
			unsigned short port = (unsigned short)strtol(listen[1].c_str(), NULL, 10);
			newSocket = server.createNewSocket(listen[0], port);
			if (newSocket == -1)
				continue ;
			Logger::info(std::cout, "Listen to port : ", port);
			ports.push_back(newSocket);
		}
	}
	tmpInfo.clear();
	if (ports.empty() == true)
		throw (ServerException("Invalid Ports."));
	return (ports);
}
