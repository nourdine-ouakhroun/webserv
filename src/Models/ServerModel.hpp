#ifndef SERVER_MODEL
#define SERVER_MODEL

#include <iostream>
#include "../parser/Location.hpp"
#include "GlobalModel.hpp"

class	ServerModel : public GlobalModel
{
	std::string	serverName;
	std::string	accessLog;
	Location	*location;

	public :
		ServerModel( void );
		ServerModel(GlobalModel&, std::string, std::string, Location*);
		ServerModel& operator=(const ServerModel& target);
		ServerModel(const ServerModel& copy);
		~ServerModel( void );
		void	setLocation(Location& _location);
		void	setServerName(std::string _serverName);
		void	setAccessLog(std::string _accessLog);

		std::string	getServerName( void ) const;
		std::string	getAccessLog( void ) const;
		Location*	getLocation( void ) const;


		
};

#endif

