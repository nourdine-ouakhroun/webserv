#include "ServerData.hpp"

ServerData::ServerData( void )
{
}

ServerData::ServerData(const vector<ServerModel>& _data) : servers(_data)
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

void	ServerData::setServerData(const vector<ServerModel>& serversData)
{
	servers = serversData;
}

void	ServerData::displayServers( void )
{
	vector<ServerModel>::iterator	iterBegin = servers.begin();
	vector<ServerModel>::iterator	iterEnd = servers.end();
	int i = 0;
	while (iterBegin < iterEnd)
	{
		cout << "\n===================== Server " << ++i << " Info =====================\n\n";
		ServerModel::printServerModelInfo(*iterBegin);
		iterBegin++;
	}
}

vector<ServerModel>	ServerData::getServersByServerName(const String& serverName)
{
	vector<ServerModel>	serv;
	vector<ServerModel>::iterator iterBegin = servers.begin();
	vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		vector<Data> value = iterBegin->getData("server_name");
		for (int i = 0; i < (int)value.size(); i++)
		{
			String str = value.at((vector<Data>::size_type)i).getValue();
			vector<String> values = str.split();
			if (find(values.begin(), values.end(), serverName) != values.end())
				serv.push_back(*iterBegin);
		}
		iterBegin++;
	}
	return (serv);
}

vector<ServerModel>	ServerData::getServersByPort(const unsigned short& port)
{
	vector<ServerModel>	serv;
	vector<ServerModel>::iterator iterBegin = servers.begin();
	vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		vector<Data> value = iterBegin->getData("listen");
		for (size_t i = 0; i < value.size(); i++)
			if (value.empty() == false && (unsigned short)strtol(value[i].getValue().c_str(), NULL, 10) == port)
				serv.push_back(*iterBegin);
		iterBegin++;
	}
	return (serv);
}

const vector<ServerModel>&	ServerData::getAllServers() const
{
	return (servers);
}

const ServerModel&	ServerData::getDefaultServer( void )
{
	vector<ServerModel>	serv;
	vector<ServerModel>::iterator iterBegin = servers.begin();
	vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		vector<Data> value = iterBegin->getData("listen");
		for (int i = 0; i < (int)value.size(); i++)
		{
			String str = value.at((vector<Data>::size_type)i).getValue();
			vector<String> values = str.split();
			if (find(values.begin(), values.end(), "default_server") != values.end())
				serv.push_back(*iterBegin);
		}
		iterBegin++;
	}
	if (serv.size() > 1)
		throw (ServerException("Duplicate default server."));
	else if (serv.size() == 0)
		serv.push_back(*servers.begin());
	return (*serv.begin());
}