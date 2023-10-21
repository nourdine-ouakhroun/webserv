#ifndef	INNERLOCATION_HPP
# define INNERLOCATION_HPP

#include <iostream>
#include <map>
#include "Location.hpp"

//class Location;

class	InnerLocation
{
	std::map<std::string, std::string> values;
//	Location location;

	public :
		InnerLocation( void );
		InnerLocation(const InnerLocation& copy);
		~InnerLocation( void );
		InnerLocation& operator=(const InnerLocation& target);
};

#endif
