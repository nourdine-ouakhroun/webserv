#ifndef	LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <map>
//#include "InnerLocation.hpp"

//class InnerLocation;

class	Location
{
	std::map<std::string, std::string> values;
	Location* innerLocation;

	public :
		Location( void );
		Location(const Location& copy);
		~Location( void );
		Location& operator=(const Location& target);
};

#endif
