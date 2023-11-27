#include"Server.hpp"

Server::Server(ServerData	srvers)
{
	this->servers = srvers;
}
void	Server::setSocket(int fd)
{
	socket(fd, )
}

void Server::runAllServers()
{
	std::vector <ServerModel> allservers =  servers.getAllServers();
	for (size_t i = 0; i < allservers.size(); i++)
	{
		std::vector <Data> ports = allservers[i].getData("listen");
		for (size_t j = 0; j < ports.size(); j++)
		{
			fdports.push_back(static_cast<int>(strtol(ports[i].getValue().c_str(),NULL, 10)));
		}
	}
	for(size_t i = 0; i < fdports.size();i++)
	{
		setSocket(fdports[i]);

	}
}

Server::~Server()
{
}