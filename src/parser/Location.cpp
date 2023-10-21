#include "Location.hpp"

Location::Location( void )
{
}

Location::Location(const Location& copy)
{
	*this = copy;
}

Location::~Location( void )
{
}

Location& Location::operator=(const Location& target)
{
	if (this != &target)
		(void)target;
	return (*this);
}
