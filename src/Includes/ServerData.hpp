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
		vector<ServerPattern> servers;
		// unsigned short	getPort(String	value);
	public :
		ServerData( void );
		ServerData(const vector<ServerPattern>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const vector<ServerPattern>&);
		void	displayServers( void );
		static vector<ServerPattern>	getServersByServerName(const vector<ServerPattern>& servers, const String& serverName);
		static vector<ServerPattern>	getServersByIpAndPort(const vector<ServerPattern>& servers, String strHost);
		static const ServerPattern&	getDefaultServer(const vector<ServerPattern>& servers);
		const vector<ServerPattern>&	getAllServers() const;

		static vector<ServerPattern>	getServer(ServerData& servers, const String& ClientAddress, String strHost);

};

#endif
