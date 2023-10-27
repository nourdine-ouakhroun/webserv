#ifndef SERVER_MODEL
#define SERVER_MODEL

#include <iostream>
#include "../parser/Location.hpp"
#include "GlobalModel.hpp"

class	ServerModel : public GlobalModel
{
	Location	*location;

	public :
		ServerModel( void );
		ServerModel(GlobalModel&, Location*);
		ServerModel& operator=(const ServerModel& target);
		ServerModel(const ServerModel& copy);
		~ServerModel( void );
		void	setLocation(Location& _location);

		Location*	getLocation( void ) const;


		
};

#endif

