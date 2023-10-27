#include "Location.hpp"

Location::Location( void ) : GlobalModel()
{
	innerLocation = NULL;
}

Location::Location(GlobalModel& model, Location* _innerLocation) :
	GlobalModel(model),
	innerLocation(_innerLocation)
{
}

Location::Location(const Location& copy)
{
	innerLocation = NULL;
	*this = copy;
}

Location::~Location( void )
{
	delete innerLocation;
}

Location& Location::operator=(const Location& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		delete innerLocation;
		innerLocation = new Location(*target.getInnerLocation());
	}
	return (*this);
}

void	Location::setInnerLocation(Location* Inner)
{
	delete innerLocation;
	innerLocation = new Location(*Inner);
}

Location*	Location::getInnerLocation( void ) const
{
	return (innerLocation);
}
