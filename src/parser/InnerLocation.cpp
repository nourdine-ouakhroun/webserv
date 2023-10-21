#include "InnerLocation.hpp"

InnerLocation::InnerLocation( void )
{
}

InnerLocation::InnerLocation(const InnerLocation& copy)
{
	*this = copy;
}

InnerLocation::~InnerLocation( void )
{
}

InnerLocation& InnerLocation::operator=(const InnerLocation& target)
{
	if (this != &target)
		(void)target;
	return (*this);
}
