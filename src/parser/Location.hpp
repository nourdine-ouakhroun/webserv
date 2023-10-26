#ifndef	LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <map>
#include "../Models/GlobalModel.hpp"
//class InnerLocation;

class	Location : public GlobalModel
{
	Location* innerLocation;

	public :
		Location( void );
		Location(GlobalModel&, Location*);
		Location(const Location& copy);
		~Location( void );
		Location& operator=(const Location& target);
		void	setInnerLocation(Location* Inner);
		Location	*getInnerLocation( void ) const;
};

#endif
