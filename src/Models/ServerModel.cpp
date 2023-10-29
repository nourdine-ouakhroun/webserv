#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(GlobalModel& model, std::vector<Location*> _location) : GlobalModel(model), location(_location)
{
}

ServerModel::~ServerModel( void ) throw()
{
	try
	{
/*		std::vector<Location*>::iterator b = location.begin();
		std::vector<Location*>::iterator e = location.end();
		while (b < e)
		{
			delete *b;
			*b = NULL;
			b++;
		}*/
		location.clear();
	}
	catch (...)
	{
		std::cerr << "catch Exception in ServerModel Destructor (~ServerModel)." << std::endl;
	}
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
//		delete location;
//		location = new Location(*target.getLocation());
	}
	return (*this);
}

void	ServerModel::setLocation(std::vector<Location*> _location)
{
	location = _location;
}

const std::vector<Location*>&	ServerModel::getLocation( void ) const
{
	return (location);
}

void	ServerModel::addLocation(Location* _location)
{
	location.push_back(_location);
}

/*
void	ServerModel::clear( void )
{
	delete location;
}
*/
