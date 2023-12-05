#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "../Parsing/Parser.hpp"
#include "ServerException.hpp"
#include <algorithm>


#define	ERROR_404	"<h1 style=\"text-align: center;\">404 Page not found.</h1>"
#define	ERROR_403	"<h1 style=\"text-align: center;\">403 Forbidden.</h1>"

class ServerData
{
	std::vector<ServerPattern> servers;

	public :
		ServerData( void );
		ServerData(const std::vector<ServerPattern>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const std::vector<ServerPattern>&);
		void	displayServers( void );
		std::vector<ServerPattern>	getServersByServerName(const String& serverName);
		std::vector<ServerPattern>	getServersByPort(const unsigned short& port);
		const std::vector<ServerPattern>&	getAllServers();
		const ServerPattern&	getDefaultServer( void );

};

#endif
