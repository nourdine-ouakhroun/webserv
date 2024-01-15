#include "Server.hpp"

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
		port = (unsigned short)strtol(value.substr(pos + 1).c_str(), NULL, 10);
	return port;
}

vector<ServerModel>	getServer(ServerData& servers, vector<Data> header)
{
	GlobalModel model(header);
	vector<ServerModel>	servModel;
	String strHost(model.getData("Host").at(0).getValue());
	vector<String> str = strHost.split(':');
	if (str.empty() == false)
	{
		servModel = servers.getServersByServerName(str.at(0));
		if (servModel.empty() == false)
			return (servModel);
	}
	long host = strtol(strHost.c_str(), NULL, 10);
	if (host != 0)
		servModel = servers.getServersByPort(getPort(model.getData("Host").at(0).getValue()));
	else
		servModel = servers.getServersByServerName(model.getData("Host").at(0).getValue());
	if (servModel.empty() == true)
		servModel.push_back(servers.getDefaultServer());
	return (servModel);
}


// vector<int>	openAllPorts(const vector<ServerModel>& serversInfo, Server& server)
// {
// 	vector<int> ports;
// 	int newSocket;
// 	for (size_t i = 0; i < serversInfo.size(); i++)
// 	{
// 		vector<Data> data = serversInfo[i].getData("listen");
// 		if (data.empty() == true)
// 			data.push_back(Data("listen", "80"));
// 		for (size_t i = 0; i < data.size(); i++)
// 		{
// 			unsigned short port = (unsigned short)strtol(data[i].getValue().c_str(), NULL, 10);
// 			newSocket = server.createNewSocket(port);
// 			if (newSocket == -1)
// 				continue ;
// 			Logger::info(cout, "Listen to port : ", port);
// 			ports.push_back(newSocket);
// 		}
// 	}
// 	if (ports.empty() == true)
// 		throw (ServerException("Invalid Ports."));
// 	return (ports);
// }
