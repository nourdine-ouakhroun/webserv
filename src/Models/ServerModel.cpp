#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(GlobalModel& model, std::string _serverName, std::string _accessLog, Location* _location) : GlobalModel(model), serverName(_serverName), accessLog(_accessLog), location(_location)
{
}

ServerModel::~ServerModel( void )
{
}

ServerModel::ServerModel(const ServerModel& copy) : GlobalModel(copy)
{
	location = NULL;
	*this = copy;
}

ServerModel& ServerModel::operator=(const ServerModel& target)
{
	if (this != &target)
	{
		serverName = target.serverName;
		accessLog = target.accessLog;
		delete location;
		location = new Location(*target.getLocation());
	}
	return (*this);
}

void	ServerModel::setLocation(Location& _location)
{
	delete location;
	location = new Location(_location);
}

void	ServerModel::setServerName(std::string _serverName)
{
	serverName = _serverName;
}


void	ServerModel::setAccessLog(std::string _accessLog)
{
	accessLog = _accessLog;
}

std::string	ServerModel::getServerName( void ) const
{
	return (serverName);
}


std::string	ServerModel::getAccessLog( void ) const
{
	return (accessLog);
}

Location*	ServerModel::getLocation( void ) const
{
	return (location);
}

