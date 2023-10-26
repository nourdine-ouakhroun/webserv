#ifndef	SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <map>
#include <vector>
#include "Location.hpp"
#include "../Models/GlobalModel.hpp"

class	Server
{
	std::vector<GlobalModel> values;
	std::vector<Location> locations;

	public :
		Server( void );
		Server(const Server& copy);
		~Server( void );
		Server& operator=(const Server& target);
/*		Server& pushToMap(const std::string& key, const std::string& value);
		Server& pushToVector(const Location& value);
		void	displayMapContent( void );
		void	displayVectorContent( void );*/
};

#endif
