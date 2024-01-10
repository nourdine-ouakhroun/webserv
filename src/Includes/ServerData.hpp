#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "../Parsing/Parser.hpp"
#include "ServerException.hpp"
#include <algorithm>


#define	ERROR_404	"<h1 style=\"text-align: center;\">404 Page not found.</h1>"
#define	ERROR_403	"<h1 style=\"text-align: center;\">403 Forbidden.</h1>"

class ServerData
{
	private :
		std::vector<ServerPattern> servers;
		// unsigned short	getPort(String	value);
	public :
		ServerData( void );
		ServerData(const std::vector<ServerPattern>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const std::vector<ServerPattern>&);
		void	displayServers( void );
		static std::vector<ServerPattern>	getServersByServerName(const std::vector<ServerPattern>& servers, const String& serverName);
		static std::vector<ServerPattern>	getServersByIpAndPort(const std::vector<ServerPattern>& servers, String strHost);
		static const ServerPattern&	getDefaultServer(const std::vector<ServerPattern>& servers);
		const std::vector<ServerPattern>&	getAllServers();

		static std::vector<ServerPattern>	getServer(ServerData& servers, int port, String strHost);

};

#endif
