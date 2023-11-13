#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(const GlobalModel& model, const std::vector<Location>& _location) : GlobalModel(model), location(_location)
{
}

ServerModel::~ServerModel( void ) throw()
{
	location.clear();
}

ServerModel::ServerModel(const ServerModel& copy) : GlobalModel(copy)
{
	*this = copy;
}

ServerModel& ServerModel::operator=(const ServerModel& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		location = target.location;
	}
	return (*this);
}

void	ServerModel::setLocation(std::vector<Location>& _location)
{
	location = _location;
}

const std::vector<Location>&	ServerModel::getLocation( void ) const
{
	return (location);
}

void	ServerModel::addLocation(Location _location)
{
	location.push_back(_location);
}

void	ServerModel::printServerModelInfo(const ServerModel& serverModel)
{
	String s("\t");
	GlobalModel::printGlobalModel(serverModel, s);
	std::cout << s << ">>>> Location Info <<<<\n";
	Location::printAllLocations(serverModel.getLocation(), s);
}
