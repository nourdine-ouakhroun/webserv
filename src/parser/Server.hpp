#ifndef	SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <map>

class	Server
{
	std::map<std::string, std::string> values;

	public :
		Server( void );
		Server(const Server& copy);
		~Server( void );
		Server& operator=(const Server& target);
};

#endif
