#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "Parser.hpp"
#include "ServerException.hpp"
#include <algorithm>


#define	ERROR_404	"<h1 style=\"text-align: center;\">404 Page not found.</h1>"
#define	ERROR_403	"<h1 style=\"text-align: center;\">403 Forbidden.</h1>"

class ServerData
{
	private :
		std::vector<ServerModel> servers;
		// unsigned short	getPort(String	value);
	public :
		ServerData( void );
		ServerData(const std::vector<ServerModel>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const std::vector<ServerModel>&);
		void	displayServers( void );
		static std::vector<ServerModel>	getServersByServerName(const std::vector<ServerModel>& servers, const String& serverName);
		static std::vector<ServerModel>	getServersByPort(const std::vector<ServerModel>& servers, const unsigned short& port);
		static const ServerModel&	getDefaultServer(const std::vector<ServerModel>& servers);
		const std::vector<ServerModel>&	getAllServers();

		static std::vector<ServerModel>	getServer(ServerData& servers, String strHost);

};

#endif
