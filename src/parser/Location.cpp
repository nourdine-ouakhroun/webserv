#include "Location.hpp"

Location::Location( void )
{
	innerLocation = NULL;
}

Location::Location(const Location& copy)
{
	*this = copy;
}

Location::~Location( void )
{
	delete innerLocation;
}

Location& Location::operator=(const Location& target)
{
	if (this != &target)
		(void)target;
	return (*this);
}
