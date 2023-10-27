#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(GlobalModel& model, Location* _location) : GlobalModel(model), location(_location)
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
		GlobalModel::operator=(target);
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

Location*	ServerModel::getLocation( void ) const
{
	return (location);
}

