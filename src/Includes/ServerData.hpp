#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "Parser.hpp"
#include "ServerException.hpp"
#include <algorithm>    // std::find


#define	ERROR_404	"<h1 style=\"text-align: center;\">404 Page not found.</h1>"
#define	ERROR_403	"<h1 style=\"text-align: center;\">403 Forbidden.</h1>"

class ServerData
{
	std::vector<ServerModel> servers;

	public :
		ServerData( void );
		ServerData(const std::vector<ServerModel>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const std::vector<ServerModel>&);
		void	displayServers( void );
		std::vector<ServerModel>	getServersByServerName(const String& serverName);
		std::vector<ServerModel>	getServersByPort(const unsigned short& port);
		const std::vector<ServerModel>&	getAllServers() const;
		const ServerModel&	getDefaultServer( void );

};

#endif
