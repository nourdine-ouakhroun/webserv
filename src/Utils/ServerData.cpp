#include "ServerData.hpp"

ServerData::ServerData( void )
{
}

ServerData::ServerData(const std::vector<ServerModel>& _data) : servers(_data)
{
}

ServerData::ServerData(const ServerData& copy)
{
	*this = copy;
}

ServerData&	ServerData::operator=(const ServerData& target)
{
	servers = target.servers;
	return (*this);
}

void	ServerData::setServerData(const std::vector<ServerModel>& serversData)
{
	servers = serversData;
}

void	ServerData::displayServers( void )
{
	std::vector<ServerModel>::iterator	iterBegin = servers.begin();
	std::vector<ServerModel>::iterator	iterEnd = servers.end();
	int i = 0;
	while (iterBegin < iterEnd)
	{
		std::cout << "\n===================== Server " << ++i << " Info =====================\n\n";
		ServerModel::printServerModelInfo(*iterBegin);
		iterBegin++;
	}
}

std::vector<ServerModel>	ServerData::getServersByServerName(const std::vector<ServerModel>& servers, const String& serverName)
{
	std::vector<ServerModel>	serv;
	std::vector<ServerModel>::const_iterator iterBegin = servers.begin();
	std::vector<ServerModel>::const_iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("server_name");
		for (int i = 0; i < (int)value.size(); i++)
		{
			String str = value.at((std::vector<Data>::size_type)i).getValue();
			std::vector<String> values = str.split();
			if (find(values.begin(), values.end(), serverName) != values.end())
				serv.push_back(*iterBegin);
		}
		iterBegin++;
	}
	return (serv);
}

std::vector<ServerModel>	ServerData::getServersByPort(const std::vector<ServerModel>& servers, const unsigned short& port)
{
	std::vector<ServerModel>	serv;
	std::vector<ServerModel>::const_iterator iterBegin = servers.begin();
	std::vector<ServerModel>::const_iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("listen");
		for (size_t i = 0; i < value.size(); i++)
			if (value.empty() == false && (unsigned short)std::strtol(value[i].getValue().c_str(), NULL, 10) == port)
				serv.push_back(*iterBegin);
		iterBegin++;
	}
	return (serv);
}

const std::vector<ServerModel>&	ServerData::getAllServers()
{
	return (servers);
}

const ServerModel&	ServerData::getDefaultServer(const std::vector<ServerModel>& servers)
{
	std::vector<ServerModel>	serv;
	std::vector<ServerModel>::const_iterator iterBegin = servers.begin();
	std::vector<ServerModel>::const_iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("listen");
		for (int i = 0; i < (int)value.size(); i++)
		{
			String str = value.at((std::vector<Data>::size_type)i).getValue();
			std::vector<String> values = str.split();
			if (find(values.begin(), values.end(), "default_server") != values.end())
				serv.push_back(*iterBegin);
		}
		iterBegin++;
	}
	if (serv.size() > 1)
		throw (ServerException("Duplicate default server."));
	else if (serv.size() == 0)
		serv.push_back(servers.front());
	return (serv.front());
}

std::vector<ServerModel>	ServerData::getServer(ServerData& servers, String strHost)
{
	std::vector<ServerModel>	servModel;
	std::vector<ServerModel>	srvs = servers.getAllServers();
	if (strHost.empty())
		return (ServerData::getServersByServerName(srvs, "\"\""));
	std::vector<String> str = strHost.split(':');
	if (str.size() == 2)
	{
		long host = std::strtol(str.back().c_str(), NULL, 10);
		if (host != 0)
		{
			servModel = ServerData::getServersByPort(srvs, (unsigned short)host);
			servModel = ServerData::getServersByServerName(servModel, str.front());
		}
		// else
		// 	servModel = ServerData::getServersByServerName(srvs, str.front());
	}
	else if (str.size() == 1)
		servModel = ServerData::getServersByServerName(srvs, str.at(0));
	if (servModel.empty() == true)
		servModel.push_back(servers.getDefaultServer(srvs));
	return (servModel);
}